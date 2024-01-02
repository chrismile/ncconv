/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2024, Christoph Neuhauser
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

#ifndef NCCONV_VOLUMEDATA_HPP
#define NCCONV_VOLUMEDATA_HPP

#include <vector>
#include <string>

class VolumeLoader;

class VolumeData {
public:
    ~VolumeData();
    void setLoader(VolumeLoader* loader);
    void setGridExtent(int _xs, int _ys, int _zs, float* _lon1d, float* _lat1d, float* _lev1d);
    void setNumTimeSteps(int _ts);
    void setEnsembleMemberCount(int _es);
    void setFieldNames(const std::vector<std::string>& _fieldNames);
    bool writeToNcFile(const std::string& filePath);

private:
    int xs = 0, ys = 0, zs = 0, ts = 0, es = 0;
    float* lon1d = nullptr, *lat1d = nullptr, *lev1d = nullptr;
    std::vector<std::string> fieldNames;
    VolumeLoader* volumeLoader = nullptr;
};

#endif //NCCONV_VOLUMEDATA_HPP
