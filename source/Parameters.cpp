#include "PairedFASTQFile.cpp"

class Parameters
{
private:
	string version, single, forward, reverse, interlaced, singleAdapter, forwardAdapter, reverseAdapter, outputDir, outputDir2;
	bool onlyIdentify, onlyRemove, trimQuality, ready, webInterface, trimNFlank;
	int minQuality, phredOffset, mismatchGlobal, maxN, rre;
	double mismatchRight, five_dr;

	int sizeQualityWindow, minLengthSeq;
	string arguments_panel;

public:
	Parameters(int argc, char *const argv[]);
	bool parseParameters();
	void printHelp();
	void printVersion();
	void printSummary();
};

Parameters::Parameters(int argc, char *const argv[])
{
	version = "1.0";
	bool help = false, version = false, webInterface = false;
	;
	onlyIdentify = false;
	onlyRemove = false;
	phredOffset = 0;

	mismatchRight = 0.5;
	mismatchGlobal = 2;

	ready = true;

	// Remoção de Ns
	trimNFlank = false;
	maxN = -1; // Inicialização 'false' da variável

	// Remoção por qualidade
	trimQuality = false;
	minQuality = 5; 	   // Valor padrão
	sizeQualityWindow = 4; // Valor padrão

	//
	minLengthSeq = 10;      // Valor padrão

	five_dr = 0.0;
	rre = false;

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
		else if (argument == "--min-quality")
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
		else if (argument == "--quality-window-size" || argument == "-qws")
		{
			sizeQualityWindow = atoi(argv[i + 1]);
			trimQuality = true;
			continue;
		}
		else if (argument == "--min-read-length" || argument == "-minlen")
		{
			minLengthSeq = atoi(argv[i + 1]);
			continue;
		}
		else if (argument == "--max-n")
		{
			maxN = atoi(argv[i + 1]);
			continue;
		}
		else if (argument == "--trim-n-flank")
		{
			trimNFlank = true;
			continue;
		}
		else if (argument == "--rem-remaining-end" || argument == "-rre")
		{
			rre = true;
			continue;
		}
		else if (argument == "--5-dist-rate" || argument == "-5dr")
		{
			five_dr = atof(argv[i + 1]);
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

		// outputDir = outputDir + "/fair_output";
		// outputDir.erase(outputDir.length() - 1);
		// outputDir2 = outputDir;

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

bool Parameters::parseParameters()
{

	// system("mkdir output");

	if (ready)
	{


		printSummary();

		struct timespec start, finish;
		double elapsed;

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

					while (s_fastq.hasNextSearchAdapters("forward"))
					{
					}

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
				else if (onlyRemove)
				{
					clock_gettime(CLOCK_MONOTONIC, &start);

					//INVERTER PADRAO
					string adapterInvert = "";
					for (int j = (singleAdapter.length() - 1); j >= 0; --j)
					{
						adapterInvert += singleAdapter[j];
					}

					int n_reads = 0;
					unsigned long long n_nucleotides_before = 0;
					unsigned long long n_nucleotides_after = 0;

					// Affecteds per quality cutting
					// Account reads that have been removed
					int count_affected_reads = 0;
					// Account bases that have been removed
					int count_n_bases_affecteds = 0;

					//
					int menor_quantitativo_bases = 0;
					int maior_quantitativo_bases = 0;
					int inic_aux = 1;

					while (s_fastq.hasNext())
					{
						if (inic_aux == 1)
						{
							menor_quantitativo_bases = s_fastq.getSequenceN();
							maior_quantitativo_bases = s_fastq.getSequenceN();
							inic_aux++;
						}

						bool read_affected = false;
						// Conta quantidade de reads e bases
						++n_reads;
						n_nucleotides_before += s_fastq.getSequenceN();

						// Registrar quantidade de bases de menor e maior leitura
						if (s_fastq.getSequenceN() < menor_quantitativo_bases)
							menor_quantitativo_bases = s_fastq.getSequenceN();
						if (s_fastq.getSequenceN() > maior_quantitativo_bases)
							maior_quantitativo_bases = s_fastq.getSequenceN();

						// +++ Removing Adapters +++
						s_fastq.removeAdapter(onlyRemove, singleAdapter, mismatchGlobal, adapterInvert, mismatchRight, five_dr, rre);

						// +++ Quality,Ns +++ Verifica parâmetros relacionados à qualidade (remoção de Ns e remoção por Qualidade)
						if (trimQuality || trimNFlank || maxN != -1)
						{
							s_fastq.trim(minQuality, sizeQualityWindow, trimNFlank, maxN);

							read_affected = s_fastq.getReadAffected();
							// If it was affected, count.
							if (read_affected)
								++count_affected_reads;
							// Count quantitie bases trimming
							count_n_bases_affecteds += s_fastq.returnNAffectedBases();
						}

						// Verificando se a read possui tamanho mínimo 'minLengthSeq', para ser considerada 
						// se sim, escreve read
						if((s_fastq.getSequenceN()) >= minLengthSeq){
							n_nucleotides_after += s_fastq.getSequenceN();
							s_fastq.write();
						}

					}

					clock_gettime(CLOCK_MONOTONIC, &finish);

					cerr << "+ Adapter Trimming +" << endl;
					s_fastq.closeOutput("onlyRemove");
					cerr << endl;

					// Se tiveram leituras afetadas pelo filtro de qualidade, exibir dados
					if (count_affected_reads > 0)
					{
						cerr << "+ Quality Trimming +" << endl;
						cerr << "No. reads affecteds: " << count_affected_reads << endl;
						cerr << "No. bases trimming: " << count_n_bases_affecteds << endl;
					}

					cerr << endl
						 << " ---- " << endl;


					cerr << "No. total reads: " << n_reads << endl;
					cerr << "No. total bases: " << n_nucleotides_before << endl;
					cerr << "No. bases remaining: " << n_nucleotides_after << endl;
					cerr << "-- No. bases trimming: " << (n_nucleotides_before-n_nucleotides_after) << endl;

					double t1 = (n_nucleotides_before-n_nucleotides_after);
					double percent_removal =  t1 / (n_nucleotides_before) * 100;
					cerr << "Removal rate (%): " << percent_removal << endl;

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;
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

					while (p_fastq.hasNextSearchAdapters())
					{
					}

					clock_gettime(CLOCK_MONOTONIC, &finish);

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;

					p_fastq.closeOutput("onlyIdentify");
				}
				else if (onlyRemove)
				{

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

					int n_reads = 0;
					unsigned long long n_nucleotides_before = 0;
					unsigned long long n_nucleotides_after = 0;

					// Affecteds per quality cutting
					// Account reads that have been removed
					int count_affected_reads_forward = 0;
					int count_affected_reads_reverse = 0;
					// Account bases that have been removed
					unsigned long long count_n_bases_affecteds_forward = 0;
					unsigned long long count_n_bases_affecteds_reverse = 0;

					while (p_fastq.hasNext())
					{
						++n_reads;
						n_nucleotides_before += p_fastq.getSequenceN();
						
						// +++ Removing Adapters +++
						p_fastq.removeAdapters(onlyRemove, forwardAdapter, reverseAdapter, mismatchGlobal, adapterInvert_f, adapterInvert_r, mismatchRight, five_dr, rre);

						// +++ Quality,Ns +++ Verifica parâmetros relacionados à qualidade (remoção de Ns e remoção por Qualidade)
						if (trimQuality || trimNFlank || maxN != -1)
						{
							p_fastq.trim(minQuality, sizeQualityWindow, trimNFlank, maxN);

							int read_affected = p_fastq.getReadAffected();

							// Verifica quais reads foram afetadas pelo corte de qualidade e registra
							// a quantidade de bases cortadas
							if (read_affected == 1)
							{
								++count_affected_reads_forward;
								count_n_bases_affecteds_forward += p_fastq.returnNAffectedBases_f();
							}
							else if (read_affected == 2)
							{
								++count_affected_reads_reverse;
								count_n_bases_affecteds_reverse += p_fastq.returnNAffectedBases_r();
							}
							else if (read_affected == 3)
							{
								++count_affected_reads_forward;
								++count_affected_reads_reverse;
								count_n_bases_affecteds_forward += p_fastq.returnNAffectedBases_f();
								count_n_bases_affecteds_reverse += p_fastq.returnNAffectedBases_r();
							}
						}

						// Verificando se tamanho de qualquer uma das reads, é maior que 'minLengthSeq', 
						// se sim, escreve read
						if(((p_fastq.getSequenceN_f()) >= minLengthSeq) || ((p_fastq.getSequenceN_r()) >= minLengthSeq)){
							n_nucleotides_after += p_fastq.getSequenceN();
							p_fastq.write();
						}						

					}

					clock_gettime(CLOCK_MONOTONIC, &finish);

					cerr << "+ Adapter Trimming +" << endl;
					p_fastq.closeOutput("onlyRemove");
					cerr << endl;

					// Se tiveram leituras afetadas pelo filtro de qualidade, exibir dados
					if (count_affected_reads_forward > 0 || count_affected_reads_reverse > 0)
					{
						cerr << "+ Quality Trimming +" << endl;
						cerr << " - Forward:" << endl;
						cerr << "No. reads affecteds: " << count_affected_reads_forward << endl;
						cerr << "No. bases trimming: " << count_n_bases_affecteds_forward << endl;

						cerr << " - Reverse:" << endl;
						cerr << "No. reads affecteds: " << count_affected_reads_reverse << endl;
						cerr << "No. bases trimming: " << count_n_bases_affecteds_reverse << endl;
					}

					cerr << endl
						 << " ---- " << endl;

					cerr << "No. total reads: " << (n_reads * 2) << endl;
					cerr << "No. total bases: " << n_nucleotides_before << endl;
					cerr << "No. bases remaining: " << n_nucleotides_after << endl;
					cerr << "-- No. bases trimming: " << (n_nucleotides_before-n_nucleotides_after) << endl;

					double t1 = (n_nucleotides_before-n_nucleotides_after);
					double percent_removal =  t1 / (n_nucleotides_before) * 100;
					cerr << "Removal rate (%): " << percent_removal << endl;

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;
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
					while (s_fastq.hasNextSearchAdapters(type))
					{
						if (cont % 2 == 0)
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
				}
				else if (onlyRemove)
				{

					clock_gettime(CLOCK_MONOTONIC, &start);

					//INVERTER PADRAO Single
					string adapterInvert_s = "";
					for (int j = (singleAdapter.length() - 1); j >= 0; --j)
					{
						adapterInvert_s += singleAdapter[j];
					}

					while (s_fastq.hasNext())
					{
						// +++ Removing Adapters +++
						s_fastq.removeAdapter(onlyRemove, singleAdapter, mismatchGlobal, adapterInvert_s, mismatchRight, five_dr, rre);

						// +++ Quality,Ns +++ Verifica parâmetros relacionados à qualidade (remoção de Ns e remoção por Qualidade)
						if (trimQuality || trimNFlank || maxN != -1)
						{
							s_fastq.trim(minQuality, trimQuality, trimNFlank, maxN);
						}

						s_fastq.write();
					}

					clock_gettime(CLOCK_MONOTONIC, &finish);

					elapsed = (finish.tv_sec - start.tv_sec);
					elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
					cerr << endl
						 << "Elapsed Time: " << elapsed << endl;

					s_fastq.closeOutput("onlyRemove");
				}
			}

			return true;
		}
	}
	return false;
}

void Parameters::printSummary()
{

	arguments_panel = "Parameters used:\n";

	arguments_panel += "+ Mode + \n";
	if(onlyIdentify)
	{
		arguments_panel += "-- Only Identify (--only-identify)\n";

		if(forward != ""){
			arguments_panel += "+ Paired Mode +\n";
			arguments_panel += "-- Forward File: "+forward+"\n";
			arguments_panel += "-- Reverse File: "+reverse+"\n";
			arguments_panel += "-- output1 (auto) : "+outputDir+"\n";
			arguments_panel += "-- output2 (auto) : "+outputDir2+"\n";
		}else if(single != ""){
			arguments_panel += "+ Single Mode +\n";
			arguments_panel += "-- Single File: "+single+"\n";
			arguments_panel += "-- output (--output) : "+outputDir+"\n";
		}

	}else if(onlyRemove){

		arguments_panel += "-- Only Remove (--only-remove)\n";

		if(forward != ""){
			arguments_panel += "+ Paired Mode +\n";
			arguments_panel += "-- Forward File: "+forward+"\n";
			arguments_panel += "-- Reverse File: "+reverse+"\n";
			arguments_panel += "-- output1 (auto) : "+outputDir+"\n";
			arguments_panel += "-- output2 (auto) : "+outputDir2+"\n";
		}else if(single != ""){
			arguments_panel += "+ Single Mode +\n";
			arguments_panel += "-- Single File: "+single+"\n";
			arguments_panel += "-- output (--output) : "+outputDir+"\n";
		}

		if(trimQuality){
			arguments_panel += "+ Trim Quality +\n";
			arguments_panel += "-- quality window size (-qws) : "+std::to_string(sizeQualityWindow)+"\n";
			arguments_panel += "-- minimum quality in window (--min-quality) : "+std::to_string(minQuality)+"\n";
			arguments_panel += "-- phred offset (-phred-offset) : "+std::to_string(phredOffset)+"\n";
			string tnf = ""; if(trimNFlank) tnf = "True"; else tnf = "False";
			arguments_panel += "-- trimming flanking N bases (--trim-n-flank) : "+tnf+"\n";
			arguments_panel += "-- maximum N to not discard read (--max-n) : "+std::to_string(maxN)+"\n";
			arguments_panel += "-- reads shorter than this length are discarded (-minlen) : "+std::to_string(minLengthSeq)+"\n";
			
		}

		arguments_panel += "+ INFO Search Adapter +\n";

		if(forward != ""){
			arguments_panel += "-- forward adapter (--forward-adapter) : "+forwardAdapter+"\n";
			arguments_panel += "-- reverse adapter (--reverse-adapter) : "+reverseAdapter+"\n";
		}else if(single != ""){
			arguments_panel += "-- single adapter (--adapter) : "+singleAdapter+"\n";
		}
		arguments_panel += "-- maximum mismatch (-mm) : "+std::to_string(mismatchGlobal)+"\n";
		arguments_panel += "-- mismatch rate in region 3' (-mmr) : "+std::to_string(mismatchRight)+"\n";
		arguments_panel += "-- 5' distance rate' (-5dr) : "+std::to_string(five_dr)+"\n";
		string tnf = ""; if(rre) tnf = "True"; else tnf = "False";
		arguments_panel += "-- removal remaining end' (-rre) : "+(tnf)+"\n";

	}

	cerr<< (arguments_panel) << endl;	

}

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
	cerr << "|--only-identify                  runs only adapter identification (without removal)" << endl;
	cerr << "|--only-remove                    runs only adapter removal (without identification)" << endl;
	cerr << "|                                need to set adapter(s) if this option is set" << endl;
	cerr << "|-qws/--quality-window-size <int> specify the size sliding window to remove per quality" << endl;
	cerr << "|                                [default: 4] (see parameter '--min-quality')" << endl;
	cerr << "|--min-quality     <int>          trim low quality bases using a sliding window based " << endl;
	cerr << "|                                approach inspired by Sickle/AdapterRemoval with the given window size." << endl;
	cerr << "|                                [default: off]" << endl;
	cerr << "|-minlen/--min-read-length <int>  reads shorter than this length are discarded following" << endl;
	cerr << "|                                trimming. [default: 0]" << endl;
	cerr << "|" << endl;
	cerr << "|> Trimming with N bases" << endl;	
	cerr << "|--trim-n-flank                   remove flanking N bases from each read. Ex: NNTGATGNNN -> TGATG " << endl;
	cerr << "|                                [default: off]" << endl;
	cerr << "|--max-n         <int>            discard reads containing more than 'max-n' ambiguous bases ('N') after " << endl;
	cerr << "|                                trimming and '--trim-n-flank'. " << endl;
	cerr << "|                                [default: off]" << endl;
	cerr << "|" << endl;
	cerr << "|> Advanced options:" << endl;
	cerr << "|--adapter     <adapter>          adapter sequence that will be removed (unpaired reads)" << endl;
	cerr << "|                                required with '--only-remove'" << endl;
	cerr << "|--forward-adapter   <adapter>    adapter sequence that will be removed" << endl;
	cerr << "|                                in the forward paired-end reads (required with --only-remove)" << endl;
	cerr << "|--reverse-adapter   <adapter>    adapter sequence that will be removed" << endl;
	cerr << "|                                in the reverse paired-end reads (required with --only-remove)" << endl;
	cerr << "|-mm/--mismatch      <int>        mismatch rate global (5'/3')" << endl;
	cerr << "|                                [default: 2] 2 bases" << endl;
	cerr << "|-mmr/--mismatch-right <0 to 0.6> mismatch rate in region 3'" << endl;
	cerr << "|                                [default: 0.5] 50% incompatibilities" << endl;
	cerr << "|-5dr/--5-dist-rate <0 to 1>      5' distance rate. Means that 0.5 the search will always start " << endl;
	cerr << "|                                from half the read [default: 0] " << endl;
	cerr << "|-rre/--rem-remaining-end         removes any base after the identified adapter [default: disabled]" << endl;
	cerr << "|                                 " << endl;
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