#include "PairedFASTQFile.cpp"
#include <thread>
#include <mutex>
#include <atomic>

class Parameters
{
private:
	string version, single, forward, reverse, interlaced, singleAdapter, forwardAdapter, reverseAdapter, outputDir, outputDir2, adapterInvert;
	bool onlyIdentify, onlyRemove, trim, trimQuality, ready, webInterface;
	int minQuality, phredOffset, mismatchGlobal;
	double mismatchRight;

public:
	Parameters(int argc, char *const argv[]);
	bool parseParameters();
	void printHelp();
	void printVersion();
	bool executeInThreads();
	void RemoveAdapters(SingleFASTQFile &s_fastq, bool onlyRemove, string singleAdapter, int mismatchGlobal, string adapterInvert, double mismatchRight);
};

Parameters::Parameters(int argc, char *const argv[])
{
	version = "1.0";
	bool help = false, version = false, webInterface = false;

	onlyIdentify = false;
	onlyRemove = false;
	trim = false;
	trimQuality = false;
	phredOffset = 0;

	mismatchRight = 0.5;
	mismatchGlobal = 2;

	ready = true;

	for (int i = 1; i < argc; i++)
	{
		string argument(argv[i]);
		if (argument == "--help" || argument == "-h")
		{
			help = true;
			continue;
		}
		else if (argument == "--version" || argument == "-v")
		{
			version = true;
			continue;
		}
		else if (argument == "--forward" || argument == "-f")
		{
			forward = argv[i + 1];
			continue;
		}
		else if (argument == "--reverse" || argument == "-r")
		{
			reverse = argv[i + 1];
			continue;
		}
		else if (argument == "--single" || argument == "-s")
		{
			single = argv[i + 1];
			continue;
		}
		else if (argument == "--interlaced" || argument == "-i")
		{
			interlaced = argv[i + 1];
			continue;
		}
		else if (argument == "--only-identify")
		{
			onlyIdentify = true;
			continue;
		}
		else if (argument == "--only-remove")
		{
			onlyRemove = true;
			continue;
		}
		else if (argument == "--trim")
		{
			trim = true;
			continue;
		}
		else if (argument == "--trim-quality")
		{
			trimQuality = true;
			minQuality = atoi(argv[i + 1]);
			continue;
		}
		else if (argument == "-phred-offset")
		{
			phredOffset = atoi(argv[i + 1]);
			continue;
		}
		else if (argument == "--adapter")
		{
			singleAdapter = argv[i + 1];
			continue;
		}
		else if (argument == "--forward-adapter")
		{
			forwardAdapter = argv[i + 1];
			continue;
		}
		else if (argument == "--reverse-adapter")
		{
			reverseAdapter = argv[i + 1];
			continue;
		}
		else if (argument == "--output" || argument == "-o")
		{
			outputDir = argv[i + 1];
			continue;
		}
		else if (argument == "--mismatch" || argument == "-mm")
		{
			mismatchGlobal = atoi(argv[i + 1]);
			continue;
		}
		else if (argument == "--mismatch-right" || argument == "-mmr")
		{
			mismatchRight = atof(argv[i + 1]);
			if (mismatchRight > 0.6)
				mismatchRight = 0.6;
			continue;
		}
		else if (argument == "--web-interface")
		{
			webInterface = true;
			break;
		}
	}

	const char *oDir = outputDir.c_str();
	DIR *dir = opendir(oDir);

	if (webInterface)
	{
		ready = false;
		system("python3 app/app.py");
	}
	else if (help)
	{
		printHelp();
		ready = false;
	}
	else if (version)
	{
		printVersion();
		ready = false;
	}
	else if (outputDir.length() == 0 || (single.length() == 0 && (forward.length() == 0 || reverse.length() == 0) && interlaced.length() == 0))
	{
		printHelp();
		ready = false;
	}
	else if (dir)
	{
		time_t rawtime;
		time(&rawtime);

		outputDir = outputDir + "/" + ctime(&rawtime);
		outputDir.erase(outputDir.length() - 1);
		outputDir2 = outputDir;

		if (onlyRemove)
		{
			outputDir.append("_1.fastq");
			outputDir2.append("_2.fastq");
		}
		else if (onlyIdentify)
		{
			outputDir.append("_1.identified");
			outputDir2.append("_2.identified");
		}

		closedir(dir);
	}
	else if (ENOENT == errno)
	{
		cerr << "Directory Does Not Exist." << endl;
		ready = false;
	}
}

bool Parameters::executeInThreads()
{
	system("mkdir output");

	// auto func = [&]() {
	// 	if (onlyIdentify)
	// 	{
	// 		cerr << "Adapters Found (Single File): " << endl;

	// 		while (s_fastq.hasNextSearchAdapters("forward"))
	// 		{
	// 		}

	// 		s_fastq.writeOnlyIdentifyHeader("single");

	// 		for (int i = 0; i < s_fastq.getAdaptersVec().size(); ++i)
	// 		{
	// 			cerr << s_fastq.getAdaptersVec()[i] << "\t" << s_fastq.getAdaptersVecQuant()[i] << endl;
	// 			string textOutputIdentify = s_fastq.getAdaptersVec()[i] + "\t" + std::to_string(s_fastq.getAdaptersVecQuant()[i]);
	// 			s_fastq.writeOnlyIdentify(textOutputIdentify);
	// 		}

	// 		s_fastq.closeOutput("onlyIdentify");
	// 	}
	// 	else if (onlyRemove)
	// 	{
	// 		// mtx.lock();
	// 		//INVERTER PADRAO
	// 		string adapterInvert = "";
	// 		for (int j = (singleAdapter.length() - 1); j >= 0; --j)
	// 		{
	// 			adapterInvert += singleAdapter[j];
	// 		}

	// 		while (s_fastq.hasNext(mtx))
	// 		{
	// 			cerr << "AQUI" << endl;
	// 			s_fastq.removeAdapter(onlyRemove, singleAdapter, mismatchGlobal, adapterInvert, mismatchRight, mtx);
	// 			s_fastq.write(mtx);
	// 		}
	// 		s_fastq.closeOutput("onlyRemove");
	// 	}
	// };

	if (ready)
	{
		if (single.length() != 0)
		{

			SingleFASTQFile s_fastq;
			mutex mtx;

			cerr << "Single File: " << single << endl;
			if (s_fastq.openFASTQInput(single, phredOffset) && s_fastq.openFASTQOutput(outputDir))
			{
				if (onlyRemove)
				{
					// mtx.lock();
					//INVERTER PADRAO
					adapterInvert = "";
					for (int j = (singleAdapter.length() - 1); j >= 0; --j)
					{
						adapterInvert += singleAdapter[j];
					}

					//  vector<thread> threadss;
					// while (s_fastq.hasNext())
					// {
					// cerr << "AQUI" << endl;
					// s_fastq.removeAdapter(onlyRemove, singleAdapter, mismatchGlobal, adapterInvert, mismatchRight);

					// threadss.push_back(std::thread(s_fastq.removeAdapter, onlyRemove, singleAdapter, mismatchGlobal, adapterInvert, mismatchRight));
					// s_fastq.write();
					// }

					auto funcy = [&](){
						s_fastq.removeAdapter(onlyRemove, singleAdapter, mismatchGlobal, adapterInvert, mismatchRight);
						s_fastq.write();
					};

					// cerr << type(funcy) << endl;

					pthread_t threads[5];
					int rc;

					for (int i = 0; i < 6; i++)
					{
						if (s_fastq.hasNext())
						{
							rc = pthread_create(&threads[i], NULL, funcy);							
						}
					}

					pthread_exit(NULL);
				}
			}
		}
	}

	return true;
}

void Parameters::RemoveAdapters(SingleFASTQFile &s_fastq, bool onlyRemove, string singleAdapter, int mismatchGlobal, string adapterInvert, double mismatchRight)
{
	cerr << "CRIOU" << endl;
	s_fastq.removeAdapter(onlyRemove, singleAdapter, mismatchGlobal, adapterInvert, mismatchRight);
	s_fastq.write();
}


// bool Parameters::parseParameters()
// {

// 	system("mkdir output");

// 	if (ready)
// 	{

// 		struct timespec start, finish;
// 		double elapsed;

// 		if (single.length() != 0)
// 		{
// 			cerr << "Single File: " << single << endl;
// 			SingleFASTQFile s_fastq;
// 			if (s_fastq.openFASTQInput(single, phredOffset) && s_fastq.openFASTQOutput(outputDir))
// 			{
// 				if (onlyIdentify)
// 				{
// 					cerr << "Adapters Found (Single File): " << endl;

// 					clock_gettime(CLOCK_MONOTONIC, &start);

// 						while(s_fastq.hasNextSearchAdapters("forward"))
// 						{}

// 						s_fastq.writeOnlyIdentifyHeader("single");

// 						for (int i = 0; i < s_fastq.getAdaptersVec().size(); ++i)
// 						{
// 							cerr << s_fastq.getAdaptersVec()[i] << "\t" << s_fastq.getAdaptersVecQuant()[i] << endl;
// 							string textOutputIdentify = s_fastq.getAdaptersVec()[i] + "\t" + std::to_string(s_fastq.getAdaptersVecQuant()[i]);
// 							s_fastq.writeOnlyIdentify(textOutputIdentify);

// 						}

// 					clock_gettime(CLOCK_MONOTONIC, &finish);

// 					elapsed = (finish.tv_sec - start.tv_sec);
// 					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

// 					cerr << endl
// 						 << "Elapsed Time: " << elapsed << endl;

// 						 s_fastq.closeOutput("onlyIdentify");	

// 				}
// 				else if(onlyRemove)
// 				{
// 					clock_gettime(CLOCK_MONOTONIC, &start);

// 					//INVERTER PADRAO
// 					string adapterInvert = "";
// 					for (int j = (singleAdapter.length() - 1); j >= 0; --j)
// 					{
// 						adapterInvert += singleAdapter[j];
// 					}

// 					while (s_fastq.hasNext())
// 					{

// 						s_fastq.removeAdapter(onlyRemove, singleAdapter, mismatchGlobal, adapterInvert, mismatchRight);

// 						if (trim)
// 						{
// 							if (trimQuality)
// 							{
// 								s_fastq.trim(minQuality, 3);
// 							}
// 							else
// 							{
// 								s_fastq.trim(-1, 1);
// 							}
// 						}

// 						s_fastq.write();
// 					}
// 					clock_gettime(CLOCK_MONOTONIC, &finish);

// 					elapsed = (finish.tv_sec - start.tv_sec);
// 					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
// 					cerr << endl
// 						 << "Elapsed Time: " << elapsed << endl;
				
// 					s_fastq.closeOutput("onlyRemove");		

// 				}

// 			}

// 			return true;
// 		}
// 		else if (forward.length() != 0 && reverse.length() != 0)
// 		{
// 			cerr << "Paired Files: " << forward << " | " << reverse << endl;
// 			PairedFASTQFile p_fastq; 
// 			if (p_fastq.openFASTQInputFile(forward, reverse, phredOffset) && p_fastq.openFASTQOutputFile(outputDir, outputDir2))
// 			{
// 				if (onlyIdentify)
// 				{
// 					cerr << "Adapters Found (Paired File): " << endl;

// 					clock_gettime(CLOCK_MONOTONIC, &start);

// 						while(p_fastq.hasNextSearchAdapters())
// 						{}

// 					clock_gettime(CLOCK_MONOTONIC, &finish);

// 					elapsed = (finish.tv_sec - start.tv_sec);
// 					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

// 					cerr << endl
// 						 << "Elapsed Time: " << elapsed << endl;

// 					p_fastq.closeOutput("onlyIdentify");

// 				}else if(onlyRemove){
					
// 					clock_gettime(CLOCK_MONOTONIC, &start);

// 					//INVERTER PADRAO FORWARD
// 					string adapterInvert_f = "";
// 					for (int j = (forwardAdapter.length() - 1); j >= 0; --j)
// 					{
// 						adapterInvert_f += forwardAdapter[j];
// 					}
// 					//INVERTER PADRAO REVERSE
// 					string adapterInvert_r = "";
// 					for (int j = (reverseAdapter.length() - 1); j >= 0; --j)
// 					{
// 						adapterInvert_r += reverseAdapter[j];
// 					}

// 					while (p_fastq.hasNext())
// 					{
// 						p_fastq.removeAdapters(onlyRemove, forwardAdapter, reverseAdapter, mismatchGlobal, adapterInvert_f, adapterInvert_r, mismatchRight);

// 						if (trim)
// 						{
// 							if (trimQuality)
// 							{
// 								p_fastq.trim(minQuality, 3);
// 							}
// 							else
// 							{
// 								p_fastq.trim(-1, 3);
// 							}
// 						}

// 						p_fastq.write();

// 					}
// 					clock_gettime(CLOCK_MONOTONIC, &finish);

// 					elapsed = (finish.tv_sec - start.tv_sec);
// 					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

// 					cerr << endl
// 						 << "Elapsed Time: " << elapsed << endl;				

// 					p_fastq.closeOutput("onlyRemove");

// 				}
// 			}

// 			return true;
// 		}
// 		else if (interlaced.length() != 0)
// 		{
// 			cerr << "Interlaced File: " << interlaced << endl;
// 				SingleFASTQFile s_fastq;
// 				if (s_fastq.openFASTQInput(interlaced, phredOffset) && s_fastq.openFASTQOutput(outputDir))
// 				{
// 					if (onlyIdentify)
// 					{
// 						cerr << "Adapters Found (Interlaced File)" << s_fastq.identifyAdapter() << endl;
// 							clock_gettime(CLOCK_MONOTONIC, &start);

// 							string type = "interlaced,forward";

// 							int cont = 1;
// 							while(s_fastq.hasNextSearchAdapters(type))
// 							{
// 									if(cont % 2 == 0)
// 									type = "interlaced,reverse";
// 									else
// 									type = "interlaced,forward";

// 								++cont;
// 							}

// 							s_fastq.writeOnlyIdentifyHeader("interlaced");

// 							for (int i = 0; i < s_fastq.getAdaptersVec().size(); ++i)
// 							{
// 								cerr << s_fastq.getAdaptersVec()[i] << "\t" << s_fastq.getAdaptersVecQuant()[i] << endl;
// 								string textOutputIdentify = s_fastq.getAdaptersVec()[i] + "\t" + std::to_string(s_fastq.getAdaptersVecQuant()[i]);
// 								s_fastq.writeOnlyIdentify(textOutputIdentify);
// 							}

// 						clock_gettime(CLOCK_MONOTONIC, &finish);

// 						elapsed = (finish.tv_sec - start.tv_sec);
// 						elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

// 						cerr << endl
// 							 << "Elapsed Time: " << elapsed << endl;

// 					s_fastq.closeOutput("onlyIdentify");

// 					}else if(onlyRemove){

// 						clock_gettime(CLOCK_MONOTONIC, &start);

// 						//INVERTER PADRAO Single
// 						string adapterInvert_s = "";
// 						for (int j = (singleAdapter.length() - 1); j >= 0; --j)
// 						{
// 							adapterInvert_s += singleAdapter[j];
// 						}

// 						while (s_fastq.hasNext())
// 						{

// 							s_fastq.removeAdapter(onlyRemove, singleAdapter, mismatchGlobal, adapterInvert_s, mismatchRight);

// 						if (trim)
// 						{
// 							if (trimQuality)
// 							{
// 								s_fastq.trim(minQuality, 3);
// 							}
// 							else
// 							{
// 								s_fastq.trim(-1, 3);
// 							}
// 						}

// 							s_fastq.write();
// 						}

// 						clock_gettime(CLOCK_MONOTONIC, &finish);

// 						elapsed = (finish.tv_sec - start.tv_sec);
// 						elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
// 						cerr << endl
// 							 << "Elapsed Time: " << elapsed << endl;


// 					s_fastq.closeOutput("onlyRemove");
					
// 					}

// 				}

// 			return true;	
// 		}
// 	}
// 	return false;
// }

void Parameters::printHelp()
{
	cerr << "  FAIR - Fast Adapter Identification and Removal v1.0" << endl;
	cerr << "" << endl;
	cerr << " Usage: /home/joao/FAIR -o <output_dir> [options]" << endl;
	cerr << "" << endl;
	cerr << "|> Basic options:" << endl;
	cerr << "|-o/--output   <output_dir>   directory to store all the resulting files (required)" << endl;
	cerr << "|-h/--help                    prints this usage message" << endl;
	cerr << "|-v/--version                 prints version" << endl;
	cerr << "|--web-interface              enable web interface in your browser for parameters selection " << endl;
	cerr << "|                            after activation, access url http://0.0.0.0:8080" << endl;
	cerr << "" << endl;
	cerr << "|> Input data:" << endl;
	cerr << "|-s/--single        <filename>    file with unpaired reads" << endl;
	cerr << "|-f/--forward       <filename>    file with forward paired-end reads" << endl;
	cerr << "|-r/--reverse       <filename>    file with reverse paired-end reads" << endl;
	cerr << "|-i/--interlaced    <filename>    file with interlaced forward and reverse paired-end reads" << endl;
	cerr << "" << endl;
	cerr << "|> Pipeline options:" << endl;
	cerr << "|--only-identify         runs only adapter identification (without removal)" << endl;
	cerr << "|--only-remove           runs only adapter removal (without identification)" << endl;
	cerr << "|                       need to set adapter(s) if this option is set" << endl;
	cerr << "|--trim                  trim ambiguous bases (N) at 5'/3' termini" << endl;
	cerr << "|--trim-quality  <int>   trim bases at 5'/3' termini and with" << endl;
	cerr << "|                       quality scores <= [--trim-quality] value" << endl;
	cerr << "|" << endl;
	cerr << "|>Advanced options:" << endl;
	cerr << "|--adapter     <adapter>          adapter sequence that will be removed (unpaired reads)" << endl;
	cerr << "|                                required with --only-remove" << endl;
	cerr << "|--forward-adapter   <adapter>    adapter sequence that will be removed" << endl;
	cerr << "|                                in the forward paired-end reads (required with --only-remove)" << endl;
	cerr << "|--reverse-adapter   <adapter>    adapter sequence that will be removed" << endl;
	cerr << "|                                in the reverse paired-end reads (required with --only-remove)" << endl;
	cerr << "|-mm/--mismatch      <int>        mismatch rate global (5'/3')" << endl;
	cerr << "|                                [default: 2] 2 bases" << endl;
	cerr << "|-mmr/--mismatch-right <0 to 0.6> mismatch rate in region 3'" << endl;
	cerr << "|                                [default: 0.5] 50% incompatibilities" << endl;
	cerr << "|--phred-offset    <33 or 64>     PHRED quality offset in the input reads (33 or 64)" << endl;
	cerr << "|                                [default: auto-detect]" << endl;
	cerr << "                                " << endl;
}
void Parameters::printVersion()
{
	cerr << endl
		 << "FAIR - Fast Adapter Identification & Removal v1.0" << endl
		 << endl;
	cerr << "Copyright (C) 2020 Federal University of Pará." << endl;
	cerr << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>." << endl;
	cerr << "This is free software: you are free to change and redistribute it." << endl;
	cerr << "There is NO WARRANTY, to the extent permitted by law." << endl
		 << endl;
	cerr << "Written by João V. Canavarro, Sebastião R. C. Neto" << endl
		 << endl;
}
