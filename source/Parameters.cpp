#include "PairedFASTQFile.cpp"

class Parameters
{
private:
	string version, single, forward, reverse, interlaced, singleAdapter, forwardAdapter, reverseAdapter, outputDir, outputDir2;
	bool onlyIdentify, onlyRemove, trim, trimQuality, ready, onlyInsert, adapterInsertionLeft, adapterRandomPosition;
	int minQuality, threads, phredOffset, mismatchGlobal;
	double adapterInsertionRate, adapterErrorRate, mismatchRight;			

public:
	Parameters(int argc, char *const argv[]);
	bool parseParameters();
	void printHelp();
	void printVersion();
};

Parameters::Parameters(int argc, char *const argv[])
{
	version = "1.0";
	bool help = false, version = false;
	onlyIdentify = false;
	onlyRemove = false;
	trim = false;
	trimQuality = false;
	threads = 1;
	phredOffset = 0;

	mismatchRight = 0.5;

	ready = true;

	mismatchGlobal = 2; // Mismatch's máximo para encontrar os adaptadores nas leituras

	// OnlyInsert (Test)
	onlyInsert = false; 
	adapterErrorRate = 0.1; // Taxa de erro máxima no adaptador(mismatch) (Default: 10%)
	adapterInsertionLeft = true; // Inserção do adaptador à direita ou esquerda
	adapterRandomPosition = false; // Posicão aleatória do adaptador (true or false) se for false, ficará na extremidade da read
	adapterInsertionRate = 0.01; // taxa de inserção dos adaptadores (Ex: 1% das reads serão infectadas)

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
		else if (argument == "--only-insert")
		{
			onlyInsert = true;
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
			continue;
		}
		else if (argument == "--min-quality")
		{
			minQuality = atoi(argv[i + 1]);
			continue;
		}
		else if (argument == "--threads" || argument == "-t")
		{
			threads = atoi(argv[i + 1]);
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
		else if (argument == "--insertion-rate" || argument == "-ir")
		{
			adapterInsertionRate = atof(argv[i + 1]);
			continue;
		}
		else if (argument == "--error-rate" || argument == "-er")
		{
			adapterErrorRate = atof(argv[i + 1]);
			continue;
		}
		else if (argument == "--insertion-left")
		{
			adapterInsertionLeft = true;
			continue;
		}
		else if (argument == "--insertion-right")
		{
			adapterInsertionLeft = false;
			continue;
		}
		else if (argument == "--random-position")
		{
			adapterRandomPosition = true;
			continue;
		}
		else if (argument == "--mismatch-right" || argument == "-mmr")
		{
			mismatchRight = atof(argv[i + 1]);
			continue;
		}
	}

	const char *oDir = outputDir.c_str();
	DIR *dir = opendir(oDir);

	if (help)
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

		if(onlyRemove)
		{
			outputDir.append("_1.fastq");
			outputDir2.append("_2.fastq");
		}else if(onlyIdentify)
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

bool Parameters::parseParameters()
{
	if (ready)
	{
		struct timespec start, finish;
		double elapsed;

		// Threads
		if (threads != 1)
		{
			;
		}

		if (single.length() != 0)
		{
			cerr << "Single File: " << single << endl;
			SingleFASTQFile s_fastq;
			if (s_fastq.openFASTQInput(single, phredOffset) && s_fastq.openFASTQOutput(outputDir))
			{
				if (onlyIdentify)
				{
					cerr << "Adapters Found (Single File): " << endl;

					clock_gettime(CLOCK_MONOTONIC, &start);

						while(s_fastq.hasNextSearchAdapters("forward"))
						{}

						s_fastq.writeOnlyIdentifyHeader("single");

						for (int i = 0; i < s_fastq.getAdaptersVec().size(); ++i)
						{
							cerr << s_fastq.getAdaptersVec()[i] << "\t" << s_fastq.getAdaptersVecQuant()[i] << endl;
							string textOutputIdentify = s_fastq.getAdaptersVec()[i] + "\t" + std::to_string(s_fastq.getAdaptersVecQuant()[i]);
							s_fastq.writeOnlyIdentify(textOutputIdentify);

						}

					clock_gettime(CLOCK_MONOTONIC, &finish);

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;

						 s_fastq.closeOutput("onlyIdentify");	

				}
				else if(onlyRemove)
				{
					clock_gettime(CLOCK_MONOTONIC, &start);

					//INVERTER PADRAO
					string adapterInvert = "";
					for (int j = (singleAdapter.length() - 1); j >= 0; --j)
					{
						adapterInvert += singleAdapter[j];
					}

					while (s_fastq.hasNext())
					{

						s_fastq.removeAdapter(onlyRemove, singleAdapter, mismatchGlobal, adapterInvert, mismatchRight);

						if (trim)
						{
							if (trimQuality)
							{
								s_fastq.trim(minQuality, 3);
							}
							else
							{
								s_fastq.trim(-1, 1);
							}
						}

						s_fastq.write();
					}
					clock_gettime(CLOCK_MONOTONIC, &finish);

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;
				
					s_fastq.closeOutput("onlyRemove");		

				}else if(onlyInsert){

					clock_gettime(CLOCK_MONOTONIC, &start);

					bool toInsert = false;
					int contAdaptersInseridos = 0;

					int rateInsertionAdapterInt = 1 / adapterInsertionRate;// convertendo taxa de inserção do adaptador
					int contador = rateInsertionAdapterInt;

					while (s_fastq.hasNext())
					{
						toInsert = false;

						// Insert at first read 
						if((contador % rateInsertionAdapterInt) == 0)
						{
							toInsert = true;
							++contAdaptersInseridos;
						}else{
							// cerr << "0" << endl;
						}

						++contador;

						s_fastq.insertAdapter(singleAdapter, toInsert, adapterErrorRate, adapterInsertionLeft, adapterRandomPosition);
						s_fastq.write();
					}

					cerr << "Inserted adapters: " << contAdaptersInseridos << endl;

					clock_gettime(CLOCK_MONOTONIC, &finish);

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;
				
					s_fastq.closeOutput("onlyInsert");	

				}

			}

			return true;
		}
		else if (forward.length() != 0 && reverse.length() != 0)
		{
			cerr << "Paired Files: " << forward << " | " << reverse << endl;
			PairedFASTQFile p_fastq; 
			if (p_fastq.openFASTQInputFile(forward, reverse, phredOffset) && p_fastq.openFASTQOutputFile(outputDir, outputDir2))
			{
				if (onlyIdentify)
				{
					cerr << "Adapters Found (Paired File): " << endl;

					clock_gettime(CLOCK_MONOTONIC, &start);

						while(p_fastq.hasNextSearchAdapters())
						{}

					clock_gettime(CLOCK_MONOTONIC, &finish);

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;

					p_fastq.closeOutput("onlyIdentify");

				}else if(onlyRemove){
					
					clock_gettime(CLOCK_MONOTONIC, &start);

					//INVERTER PADRAO FORWARD
					string adapterInvert_f = "";
					for (int j = (forwardAdapter.length() - 1); j >= 0; --j)
					{
						adapterInvert_f += forwardAdapter[j];
					}
					//INVERTER PADRAO REVERSE
					string adapterInvert_r = "";
					for (int j = (reverseAdapter.length() - 1); j >= 0; --j)
					{
						adapterInvert_r += reverseAdapter[j];
					}

					while (p_fastq.hasNext())
					{
						p_fastq.removeAdapters(onlyRemove, forwardAdapter, reverseAdapter, mismatchGlobal, adapterInvert_f, adapterInvert_r, mismatchRight);

						if (trim)
						{
							if (trimQuality)
							{
								p_fastq.trim(minQuality, 3);
							}
							else
							{
								p_fastq.trim(-1, 3);
							}
						}

						p_fastq.write();

					}
					clock_gettime(CLOCK_MONOTONIC, &finish);

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;				

					p_fastq.closeOutput("onlyRemove");

				}else if(onlyInsert){

					cerr << "Only Insert" << endl;

					clock_gettime(CLOCK_MONOTONIC, &start);

					bool toInsert = false;
					int contAdaptersInseridos = 0;

					int rateInsertionAdapterInt = 1 / adapterInsertionRate;// convertendo taxa de inserção do adaptador
					int contador = rateInsertionAdapterInt;

					while (p_fastq.hasNext())
					{
						
						toInsert = false;

						if((contador % rateInsertionAdapterInt) == 0)
						{
							toInsert = true;
							++contAdaptersInseridos;
						}

						++contador;

						p_fastq.insertAdapters(forwardAdapter, reverseAdapter, toInsert, adapterErrorRate, adapterInsertionLeft, adapterRandomPosition);
						p_fastq.write();

					}

					cerr << "Inserted adapters: " << contAdaptersInseridos*2 << endl;

					clock_gettime(CLOCK_MONOTONIC, &finish);

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;
				
					p_fastq.closeOutput("onlyInsert");	

				}
			}

			return true;
		}
		else if (interlaced.length() != 0)
		{
			cerr << "Interlaced File: " << interlaced << endl;
				SingleFASTQFile s_fastq;
				if (s_fastq.openFASTQInput(interlaced, phredOffset) && s_fastq.openFASTQOutput(outputDir))
				{
					if (onlyIdentify)
					{
						cerr << "Adapters Found (Interlaced File)" << s_fastq.identifyAdapter() << endl;
							clock_gettime(CLOCK_MONOTONIC, &start);

							string type = "interlaced,forward";

							int cont = 1;
							while(s_fastq.hasNextSearchAdapters(type))
							{
									if(cont % 2 == 0)
									type = "interlaced,reverse";
									else
									type = "interlaced,forward";

								++cont;
							}

							s_fastq.writeOnlyIdentifyHeader("interlaced");

							for (int i = 0; i < s_fastq.getAdaptersVec().size(); ++i)
							{
								cerr << s_fastq.getAdaptersVec()[i] << "\t" << s_fastq.getAdaptersVecQuant()[i] << endl;
								string textOutputIdentify = s_fastq.getAdaptersVec()[i] + "\t" + std::to_string(s_fastq.getAdaptersVecQuant()[i]);
								s_fastq.writeOnlyIdentify(textOutputIdentify);
							}

						clock_gettime(CLOCK_MONOTONIC, &finish);

						elapsed = (finish.tv_sec - start.tv_sec);
						elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

						cerr << endl
							 << "Elapsed Time: " << elapsed << endl;

					s_fastq.closeOutput("onlyIdentify");

					}else if(onlyRemove){

						clock_gettime(CLOCK_MONOTONIC, &start);

						//INVERTER PADRAO Single
						string adapterInvert_s = "";
						for (int j = (singleAdapter.length() - 1); j >= 0; --j)
						{
							adapterInvert_s += singleAdapter[j];
						}

						while (s_fastq.hasNext())
						{

							s_fastq.removeAdapter(onlyRemove, singleAdapter, mismatchGlobal, adapterInvert_s, mismatchRight);

						if (trim)
						{
							if (trimQuality)
							{
								s_fastq.trim(minQuality, 3);
							}
							else
							{
								s_fastq.trim(-1, 3);
							}
						}

							s_fastq.write();
						}

						clock_gettime(CLOCK_MONOTONIC, &finish);

						elapsed = (finish.tv_sec - start.tv_sec);
						elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
						cerr << endl
							 << "Elapsed Time: " << elapsed << endl;


					s_fastq.closeOutput("onlyRemove");
					
					}else if(onlyInsert){

					clock_gettime(CLOCK_MONOTONIC, &start);

					bool toInsert = false;
					int contAdaptersInseridos = 0;

					int rateInsertionAdapterInt = 1 / adapterInsertionRate;// convertendo taxa de inserção do adaptador
					int contador = rateInsertionAdapterInt;

					while (s_fastq.hasNext())
					{
						toInsert = false;

						if((contador % rateInsertionAdapterInt) == 0)
						{
							toInsert = true;
							++contAdaptersInseridos;
						}

						++contador;
						// cerr << "cont: " << contador << endl; 
						s_fastq.insertAdapter(singleAdapter, toInsert, adapterErrorRate, adapterInsertionLeft, adapterRandomPosition);
						s_fastq.write();
					}

					cerr << "Inserted adapters: " << contAdaptersInseridos << endl;

					clock_gettime(CLOCK_MONOTONIC, &finish);

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;
				
					s_fastq.closeOutput("onlyInsert");	

					}

				}

			return true;	
		}
	}
	return false;
}
void Parameters::printHelp()
{
	cerr <<"  FAIIR - Fast Adapter Identification, Insertion and Removal v1.0" << endl;
	cerr << "" << endl;
	cerr << " Usage: /home/joao/FAIR -o <output_dir> [options]" << endl;
	cerr << "" << endl;
	cerr << "|> Basic options:" << endl;
	cerr << "|-o/--output   <output_dir>   directory to store all the resulting files (required)" << endl;
	cerr << "|-h/--help                    prints this usage message" << endl;
	cerr << "|-v/--version                 prints version" << endl;
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
	cerr << "|                        need to set adapter(s) if this option is set" << endl;
	cerr << "|--trim                  trim ambiguous bases (N) at 5'/3' termini" << endl;
	cerr << "|--trim-quality          trim bases at 5'/3' termini with quality scores <= to" << endl;
	cerr << "|                        --min-quality value" << endl;
	cerr << "|--min-quality   <int>   minimal quality value to trim" << endl;
	cerr << "" << endl;
	cerr << "|>Advanced options:" << endl;
	cerr << "|--adapter     <adapter>          adapter sequence that will be removed (unpaired reads)" << endl;
	cerr << "|                                required with --only-remove" << endl;
	cerr << "|--forward-adapter   <adapter>    adapter sequence that will be removed" << endl;
	cerr << "|                                in the forward paired-end reads (required with --only-remove)" << endl;
	cerr << "|--reverse-adapter   <adapter>    adapter sequence that will be removed" << endl;
	cerr << "|                                in the reverse paired-end reads (required with --only-remove)" << endl;
	cerr << "|-mm/--mismatch      <int>        mismatch rate global (5'/3')" << endl;
	cerr << "|                                [default: 2] 2 bases" << endl;
	cerr << "|-mmr/--mismatch-right            mismatch rate in region 3'" << endl;
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
	cerr << "Written by João V. Canavarro and Sebastião R. C. Neto" << endl
		 << endl;
}
