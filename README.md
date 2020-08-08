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
  <a href="https://github.com/SRodriguess/fair">
    <img src="logo.png" alt="Logo" width="250" height="250">
  </a>

  <h3 align="center">Fast Adapter Identification & Removal</h3>

  <p align="center">
    <br />
    <a href="https://github.com/SRodriguess/FAIR"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/SRodriguess/FAIR">View Demo</a>
    ·
    <a href="https://github.com/SRodriguess/FAIR/issues">Report Bug</a>
    ·
    <a href="https://github.com/SRodriguess/FAIR/issues">Request Feature</a>
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
* [Acknowledgements and References](#acknowledgements-and-references)
* [Contact](#contact)



<!-- ABOUT THE PROJECT -->
## About The Project

FAIR is an adapter cutting tool that uses the algorithms of Wu & Manber (1992) and Myers (1999) that take advantage of efficient chain matching techniques, providing a tool that runs in O (n+m) time for removal of adapters.

### Built With
The project was built mainly with C++, but some funcionalities are based on python scripts
* [C++](http://www.cplusplus.org)
* [Python](https://www.python.org)


<!-- GETTING STARTED -->
## Getting Started

FAIR works with single fastq files, forward / reverse and interlaced, to identify, trim and remove low quality N / adapters and bases from the strings. In addition to the command interface (CLI), FAIR can be used through a graphical web interface, developed in Flask Python for parameter selection.
At the end of the execution, a new fastq file is created in the directory chosen by the user with the adapter segments removed and an additional file with the excluded bases. FAIR does not yet work with tar.gz files.

### Prerequisites

This repository can be built with any C++ compiler. During the conception of the project we used gcc with any major problem. Additionally, Python is necessary for some extra funcionalities.
* gcc
```sh
sudo apt-get install gcc
```
* python
```sh
sudo apt-get install python
```

If you want to execute algorithm evaluation located on `utils` some extra Python Frameworks are required, namely: pandas, matplotlib and numpy. Thankfully, you can * * install them all at once using pip.
* pip
```sh
pip install -r requirements.txt --user
```

If graphic web interface is required
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
git clone https://github.com/SRodriguess/FAIR.git
```
2. Build with compiler
```sh
cd FAIR
g++ source/main.cpp -o FAIR
```


<!-- USAGE EXAMPLES -->
## Usage

### CLI (Command Line Interface)

Bellow are listed all FAIR avaiable parameters.

```FAIR - Fast Adapter Identification and Removal v1.0

 Usage: /home/joao/FAIR -o <output_dir> [options]

|> Basic options:
|-o/--output   <output_dir>   directory to store all the resulting files (required)
|-h/--help                    prints this usage message
|-v/--version                 prints version

|> Input data:
|-s/--single        <filename>    file with unpaired reads
|-f/--forward       <filename>    file with forward paired-end reads
|-r/--reverse       <filename>    file with reverse paired-end reads
|-i/--interlaced    <filename>    file with interlaced forward and reverse paired-end reads

|> Pipeline options:
|--only-identify         runs only adapter identification (without removal)
|--only-remove           runs only adapter removal (without identification)
|                        need to set adapter(s) if this option is set
|--trim                  trim ambiguous bases (N) at 5'/3' termini
|--trim-quality          trim bases at 5'/3' termini with quality scores <= to
|                        --min-quality value
|--min-quality   <int>   minimal quality value to trim

|>Advanced options:
|--adapter     <adapter>          adapter sequence that will be removed (unpaired reads)
|                                required with --only-remove
|--forward-adapter   <adapter>    adapter sequence that will be removed
|                                in the forward paired-end reads (required with --only-remove)
|--reverse-adapter   <adapter>    adapter sequence that will be removed
|                                in the reverse paired-end reads (required with --only-remove)
|-mm/--mismatch      <int>        mismatch rate global (5'/3')
|                                [default: 2] 2 bases
|-mmr/--mismatch-right            mismatch rate in region 3'
|                                [default: 0.5] 50% incompatibilities
|--phred-offset    <33 or 64>     PHRED quality offset in the input reads (33 or 64)
|                                [default: auto-detect]
```

### Web Interface

Do you use to select parameters more easily.

1. Running Server
```sh
python3 app/app.py
```
2. Access URL in your browser http://0.0.0.0:8080/

_For more examples, please refer to the [Documentation](https://github.com/SRodriguess/FAIR)_

## Examples
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

* Remove Adapters from Single FASTQ File with Trim, Trim-Quality, Min-Quality and Number of Phred-Offset
```sh
./FAIR --only-remove --single sample1.fastq --trim --trim-quality 90 --min-quality 90 --phread-offset 33 --output results/
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
## Acknowledgements and References
* [R. S. Boyer and J. S. Moore. A fast string searching algorithm. Commun. ACM,20(10):762–772, 1977.](https://www.cs.utexas.edu/users/moore/publications/fstrpos.pdf)
* [Koloud Al-Khamaiseh  and  ShadiALShagarin, “A Survey of  String  Matching  Algorithms”  in  Int.  Journal  of Engineering  Research  and Applications,  IJERA,  ISSN: 2248-9622,  Vol.  4,  Issue  7  (Version  2),  July  2014, pp.144-156](https://core.ac.uk/download/pdf/26834073.pdf)
* [B. Durian, H. Peltola, L. Salmela, and J. Tarhio. Bit-parallel search algorithms forlong patterns. In P. Festa, editor, Symposium on Experimental Algorithms, LNCS6049, 129–140, Springer-Verlag, Berlin, 2010.](https://helda.helsinki.fi/bitstream/handle/10138/18570/Long.pdf?sequence=2)
* [G. Navarro, M. Raffinot, “A Bit-Parallel Approach to Suffix Automata: Fast Extended String Matching”, in Proc. of the 9th Annual Symposium on Combinatorial Pattern Matching, No. 1448.](https://users.dcc.uchile.cl/~gnavarro/ps/cpm98.pdf)
* [SMART (String Matching Algorithm Research Tool)](https://github.com/smart-tool/smart)
* [bio-playground by Brent Perdensen](https://github.com/brentp/bio-playground)

<!-- CONTACT -->
## Contact

Sebastião R. C. Neto - s.rodriguescn@gmail.com

Project Link: [https://github.com/SRodriguess/FAIR](https://github.com/SRodriguess/FAIR)

<!-- MARKDOWN LINKS & IMAGES -->
[build-shield]: https://img.shields.io/badge/build-passing-brightgreen.svg?style=flat-square
[contributors-shield]: https://img.shields.io/badge/contributors-1-orange.svg?style=flat-square
[license-shield]: https://img.shields.io/badge/license-MIT-blue.svg?style=flat-square
[license-url]: https://choosealicense.com/licenses/mit
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=flat-square&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/jvcanavarro/
[product-screenshot]:https://github.com/jvcanavarro/FAIR-Fast-Adapter-Identification-and-Removal/blob/master/utils/best5.png
