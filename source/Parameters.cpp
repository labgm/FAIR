#include "PairedFASTQFile.cpp"

class Parameters
{
private:
	string version, single, forward, reverse, interlaced, singleAdapter, forwardAdapter, reverseAdapter, outputDir, outputDir2;
	bool onlyIdentify, onlyRemove, trim, trimQuality, ready;
	int minQuality, threads, phredOffset;

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
	else if (outputDir.length() == 0 || (single.length() == 0 && (forward.length() == 0 || reverse.length() == 0)))
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
		outputDir.append("_1.fastq");
		outputDir2.append("_2.fastq");

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
					cerr << "Adapter (Single File)" << s_fastq.identifyAdapter() << endl;
				}
				else
				{
					clock_gettime(CLOCK_MONOTONIC, &start);

					while (s_fastq.hasNext())
					{

						s_fastq.removeAdapter(onlyRemove, singleAdapter);

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
				}
			}
			s_fastq.closeOutput();

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
					cerr << "Adapters (Paired File): " << endl;
				}
				else
				{
					clock_gettime(CLOCK_MONOTONIC, &start);

					while (p_fastq.hasNext())
					{
						p_fastq.removeAdapters(onlyRemove, forwardAdapter, reverseAdapter);

						if (trim)
						{
							if (trimQuality)
							{
								p_fastq.trim(minQuality, 3);
							}
							else
							{
								p_fastq.trim(-1, 1);
							}
						}

						p_fastq.write();
					}
					clock_gettime(CLOCK_MONOTONIC, &finish);

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

					// cerr << "Total de bases: " << totalBase << endl;
					// cerr << "Aproximadamente: " << totalBase/elapsed << endl;

					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;
				}
			}
			p_fastq.closeOutput();

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
						cerr << "Adapter (Interlaced File)" << s_fastq.identifyAdapter() << endl;
					}
					else
					{
						clock_gettime(CLOCK_MONOTONIC, &start);

						while (s_fastq.hasNext())
						{

							s_fastq.removeAdapter(onlyRemove, singleAdapter);

							if (trim)
							{
								s_fastq.trim(minQuality, 0);
							}

							s_fastq.write();
						}
						clock_gettime(CLOCK_MONOTONIC, &finish);

						elapsed = (finish.tv_sec - start.tv_sec);
						elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
						cerr << endl
							 << "Elapsed Time: " << elapsed << endl;
					}
				}
				s_fastq.closeOutput();

			return true;	
		}
	}
	return false;
}
void Parameters::printHelp()
{
	cerr <<" FAIR - Fast Adapter Identification and Removal v1.0" << endl;
	cerr << "" << endl;
	cerr << "Usage: /home/joao/FAIR -o <output_dir> [options]" << endl;
	cerr << "" << endl;
	cerr << "Basic options:" << endl;
	cerr << "-o/--output   <output_dir>   directory to store all the resulting files (required)" << endl;
	cerr << "-h/--help                    prints this usage message" << endl;
	cerr << "-v/--version                 prints version" << endl;
	cerr << "" << endl;
	cerr << "Input data:" << endl;
	cerr << "-s/--single        <filename>    file with unpaired reads" << endl;
	cerr << "-f/--forward       <filename>    file with forward paired-end reads" << endl;
	cerr << "-r/--reverse       <filename>    file with reverse paired-end reads" << endl;
	cerr << "-i/--interlaced    <filename>    file with interlaced forward and reverse paired-end reads" << endl;
	cerr << "" << endl;
	cerr << "Pipeline options:" << endl;
	cerr << "--only-identify         runs only adapter identification (without removal)" << endl;
	cerr << "--only-remove           runs only adapter removal (without identification)" << endl;
	cerr << "                        need to set adapter(s) if this option is set" << endl;
	cerr << "--trim                  trim ambiguous bases (N) at 5'/3' termini" << endl;
	cerr << "--trim-quality          trim bases at 5'/3' termini with quality scores <= to" << endl;
	cerr << "                        --min-quality value" << endl;
	cerr << "--min-quality   <int>   minimal quality value to trim" << endl;
	cerr << "" << endl;
	cerr << "Advanced options:" << endl;
	cerr << "--adapter     <adapter>         adapter sequence that will be removed (unpaired reads)" << endl;
	cerr << "                                required with --only-remove" << endl;
	cerr << "--forward-adapter   <adapter>   adapter sequence that will be removed" << endl;
	cerr << "                                in the forward paired-end reads (required with --only-remove)" << endl;
	cerr << "--reverse-adapter   <adapter>   adapter sequence that will be removed" << endl;
	cerr << "                                in the reverse paired-end reads (required with --only-remove)" << endl;
	cerr << "-t/--threads    <int>           number of threads" << endl;
	cerr << "                                [default: 1]" << endl;
	cerr << "--phred-offset    <33 or 64>    PHRED quality offset in the input reads (33 or 64)" << endl;
	cerr << "                                [default: auto-detect]" << endl;
	cerr << "                                " << endl;
}
void Parameters::printVersion()
{
	cerr << endl
		 << "FAIR - Fast Adapter Identification & Removal v1.0" << endl
		 << endl;
	cerr << "Copyright (C) 2019 Federal University of Pará." << endl;
	cerr << "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>." << endl;
	cerr << "This is free software: you are free to change and redistribute it." << endl;
	cerr << "There is NO WARRANTY, to the extent permitted by law." << endl
		 << endl;
	cerr << "Written by João V. Canavarro." << endl
		 << endl;
}
