<!--
*** Thanks for checking out this README Template. If you have a suggestion that would
*** make this better please fork the repo and create a pull request or simple open
*** an issue with the tag "enhancement".
*** Thanks again! Now go create something AMAZING! :D
-->

<!-- PROJECT SHIELDS -->
[![Build Status][build-shield]]()
[![Contributors][contributors-shield]]()
[![MIT License][license-shield]][license-url]
[![LinkedIn][linkedin-shield]][linkedin-url]



<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/engbiopct/FAIR">
    <img src="app/static/images/icons/logo.png" alt="Logo" width="150">
  </a>

  <h3 align="center">Fast Adapter Identification & Removal</h3>

  <p align="center">
    <br />
    <a href="https://github.com/engbiopct/FAIR"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/engbiopct/FAIR">View Demo</a>
    ·
    <a href="https://github.com/engbiopct/FAIR/issues">Report Bug</a>
    ·
    <a href="https://github.com/engbiopct/FAIR/issues">Request Feature</a>
  </p>
</p>



<!-- TABLE OF CONTENTS -->
## Table of Contents

* [About the Project](#about-the-project)
  * [Built With](#built-with)
* [Getting Started](#getting-started)
  * [Prerequisites](#prerequisites)
  * [Installation](#installation)
* [Usage](#usage)
* [Examples](#examples)
* [Contributing](#contributing)
* [License](#license)
* [Acknowledgements and References](#acknowledgements)
* [Contact](#contact)



<!-- ABOUT THE PROJECT -->
## About The Project

FAIR is an adapter cutting tool that uses the algorithms of Wu & Manber (1992) and Myers (1999) that take advantage of efficient chain matching techniques, providing a tool that runs in O (n+m) time for removal of adapters.

### Built With
The project was built mainly with C++, but some funcionalities are based on python scripts. JS, CSS and HTML are used for web interface, for parameter selecion.
* [C++](http://www.cplusplus.org)
* [Python](https://www.python.org)


<!-- GETTING STARTED -->
## Getting Started

FAIR works with single fastq files, forward / reverse and interlaced, to identify, trim and remove low quality N / adapters and bases from the strings. In addition to the command interface (CLI), FAIR can be used through a graphical web interface, developed in Flask Python for parameter selection.
At the end of the execution, when selecting the option to remove the adapters a new fastq file is created in the directory chosen by the user with the adapter segments removed. FAIR does not yet work with tar.gz files.

### Prerequisites

This repository can be built with any C++ compiler. During the conception of the project we used gcc with any major problem. Additionally, Python is necessary for some extra functionalities.
* gcc
```sh
sudo apt-get install gcc
```
* python
```sh
sudo apt-get install python
```

If graphic web interface is required (Parameter [--web-interface])
* Python Flask
```sh
sudo apt install python3-flask
```
* Python FlaskRestful
```sh
pip3 install flask-restful
```

### Binary Compile(if you have made changes to the source code)

1. Clone the repo
```sh
git clone https://github.com/engbiopct/FAIR.git
```
2. Make changes in source code (files in the 'source' folder)

3. Build with compiler
```sh
cd FAIR
g++ source/main.cpp -o FAIR
```

<!-- USAGE EXAMPLES -->
## Usage

### CLI (Command Line Interface)

Bellow are listed all FAIR available parameters.

```FAIR - Fast Adapter Identification and Removal v1.0

  Usage: /home/joao/FAIR -o <output_dir> [options]

|> Basic options:
|-o/--output   <output_dir>   directory to store all the resulting files (required)
|-h/--help                    prints this usage message
|-v/--version                 prints version
|--web-interface              enable web interface in your browser for parameters selection 
|                            after activation, access url http://0.0.0.0:8080

|> Input data:
|-s/--single        <filename>    file with unpaired reads
|-f/--forward       <filename>    file with forward paired-end reads
|-r/--reverse       <filename>    file with reverse paired-end reads
|-i/--interlaced    <filename>    file with interlaced forward and reverse paired-end reads

|> Pipeline options:
|--only-identify                  runs only adapter identification (without removal)
|--only-remove                    runs only adapter removal (without identification)
|                                need to set adapter(s) if this option is set
|-qws/--quality-window-size <int> specify the size sliding window to remove per quality
|                                [default: 4] (see parameter '--min-quality')
|--min-quality     <int>          trim low quality bases using a sliding window based 
|                                approach inspired by Sickle/AdapterRemoval with the given window size.
|                                [default: 10]
|-minlen/--min-read-length <int>  reads shorter than this length are discarded following
|                                trimming. [default: 0]
|
|> Trimming with N bases  
|--trim-n-flank                   remove flanking N bases from each read. Ex: NNTGATGNNN -> TGATG 
|                                [default: off]
|--max-n         <int>            discard reads containing more than 'max-n' ambiguous bases ('N') after 
|                                trimming and '--trim-n-flank'. 
|                                [default: off]
|
|> Advanced options:
|--adapter     <adapter>          adapter sequence that will be removed (unpaired reads)
|                                required with '--only-remove'
|--forward-adapter   <adapter>    adapter sequence that will be removed
|                                in the forward paired-end reads (required with --only-remove)
|--reverse-adapter   <adapter>    adapter sequence that will be removed
|                                in the reverse paired-end reads (required with --only-remove)
|-mm/--mismatch      <int>        mismatch rate global (5'/3')
|                                [default: 2] 2 bases
|-mmr/--mismatch-right <0 to 0.6> mismatch rate in region 3'
|                                [default: 0.5] 50% incompatibilities
|-5dr/--5-dist-rate <0 to 1>      5' distance rate. Means that 0.5 the search will always start 
|                                from half the read [default: 0] 
|-rre/--rem-remaining-end         removes any base after the identified adapter [default: disabled]
|                                 
|--phred-offset    <33 or 64>     PHRED quality offset in the input reads (33 or 64)
|                                [default: auto-detect]
```

### Web Interface

Do you use to select parameters.

1. Running Server
```sh
cd FAIR
./FAIR --web-interface
```
2. Access URL in your browser http://0.0.0.0:8080/

_For more examples, please refer to the [Documentation](https://github.com/engbiopct/FAIR)_

## Examples (CLI)
You can test the program utilizing the samples `sample1.fastq` and `sample2.fastq` located at `data`. The new files are stored on `results`. Some common usages are listed bellow.

* Remove Adapters from Single FASTQ File with Adapter
```sh
./FAIR --only-remove --single sample1.fastq --adapter CCTCCTCCTCCT --output results/
```

* Remove Adapters from Forward and Reverse FASTQ Files
```sh
./FAIR --only-remove --forward sample1.fastq --forward-adapter CCCCCCC --reverse sample2.fastq --reverse-adapter CCCATCC --output results/
```

* Identify Adapters from Forward and Reverse FASTQ Files
```sh
./FAIR --only-identify --forward sample1.fastq --reverse sample2.fastq --output results/
```

* Remove Adapters from Single FASTQ File with Mismatch Global 5'/3' (3 nucleotides); discard reads with more than 10 Ns; Minimum window quality 30, Number of Phred-Offset; maximum differences in region 3' (30%).
```sh
./FAIR --only-remove --single sample1.fastq --adapter AGAGAATAGATAGAT --mismatch 3 --max-n 10 --min-quality 30 --phread-offset 33 --mismatch-right 0.3 --output results/
```

<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to be learn, inspire, and create. Any contributions you make are **greatly appreciated**.

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE` for more information.


<!-- ACKNOWLEDGEMENTS -->
## Acknowledgements
We thank the research group EngBio PCT-Guamá. This work was supported by Coordenação de Aperfeiçoamento de Pessoal de Nível Superior (CAPES).

## References
* [Hyyro, H., Fredriksson, K., \& Navarro, G. (2005). Increased bit-parallelism for approximate and multiple string matching. Journal of Experimental Algorithmics (JEA), 10, 2-6.](https://dl.acm.org/doi/pdf/10.1145/1064546.1180617)
* [Myers, G. (1999). A fast bit-vector algorithm for approximate string matching based on dynamic programming. Journal of the ACM (JACM), 46(3), 395-415.](https://dl.acm.org/doi/10.1145/316542.316550)
* [Wu, S., \& Manber, U. (1992). Fast text searching: allowing errors. Communications of the ACM, 35(10), 83-91.](https://dl.acm.org/doi/10.1145/135239.135244)
* [bio-playground by Brent Perdensen](https://github.com/brentp/bio-playground)
* [Andrews, S. (2010). FastQC: a quality control tool for high throughput sequence data.](https://www.bioinformatics.babraham.ac.uk/projects/fastqc/)

<!-- CONTACT -->
## Contact

Sebastião R. C. Neto - s.rodriguescn@gmail.com

Project Link: [https://github.com/engbiopct/FAIR](https://github.com/engbiopct/FAIR)

<!-- MARKDOWN LINKS & IMAGES -->
[build-shield]: https://img.shields.io/badge/build-passing-brightgreen.svg?style=flat-square
[contributors-shield]: https://img.shields.io/badge/contributors-1-orange.svg?style=flat-square
[license-shield]: https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square
[license-url]: https://choosealicense.com/licenses/mit
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555