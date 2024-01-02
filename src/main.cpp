/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2023, Christoph Neuhauser
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>

#include "Utils/StringUtils.hpp"
#include "Loaders/CtlLoader.hpp"
#include "Volume/VolumeData.hpp"

void printHelp() {
    std::cout << "Supported options:" << std::endl;
    std::cout << "--input or -i: Path to the input file." << std::endl;
    std::cout << "--output or -o: Path to the output file." << std::endl;
}

int main(int argc, char *argv[]) {
    std::string inputFile, outputFile;
    for (int i = 1; i < argc; i++) {
        std::string command = argv[i];
        if (command == "--input" || command == "-i") {
            i++;
            if (i >= argc) {
                throw std::runtime_error("Error: Command line arguments '--input' and '-i' expect a file path.");
            }
            inputFile = argv[i];
        } else if (command == "--output" || command == "-o") {
            i++;
            if (i >= argc) {
                throw std::runtime_error("Error: Command line arguments '--input' and '-i' expect a file path.");
            }
            outputFile = argv[i];
        } else if (command == "--help" || command == "-h") {
            printHelp();
        }
    }

    if (inputFile.empty() || outputFile.empty()) {
        throw std::runtime_error("Error: Input or output file path not specified. Use '--help' for more information.");
    }

    CtlLoader* loader = nullptr;
    if (sgl::endsWith(inputFile, ".ctl")) {
        loader = new CtlLoader;
    } else {
        throw std::runtime_error("Error: Unsupported input file extension.");
    }

    VolumeData* volumeData = new VolumeData;
    std::cout << "Opening input file..." << std::endl;
    if (!loader->setInputFiles(volumeData, inputFile, {})) {
        throw std::runtime_error("Error: Parsing input file format failed.");
    }
    volumeData->setLoader(loader);
    std::cout << "Writing output file..." << std::endl;
    volumeData->writeToNcFile(outputFile);
    delete volumeData;
    delete loader;

    return 0;
}
