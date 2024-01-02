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

#include <iostream>
#include <stdexcept>

#include <netcdf.h>

#include "Loaders/VolumeLoader.hpp"
#include "VolumeData.hpp"

VolumeData::~VolumeData() {
    if (lon1d) {
        delete[] lon1d;
    }
    if (lat1d) {
        delete[] lat1d;
    }
    if (lev1d) {
        delete[] lev1d;
    }
}

void VolumeData::setLoader(VolumeLoader* loader) {
    volumeLoader = loader;
}

void VolumeData::setGridExtent(int _xs, int _ys, int _zs, float* _lon1d, float* _lat1d, float* _lev1d) {
    xs = _xs;
    ys = _ys;
    zs = _zs;
    lon1d = _lon1d;
    lat1d = _lat1d;
    lev1d = _lev1d;
}

void VolumeData::setNumTimeSteps(int _ts) {
    ts = _ts;
}

void VolumeData::setEnsembleMemberCount(int _es) {
    es = _es;
}

void VolumeData::setFieldNames(const std::vector<std::string>& _fieldNames) {
    fieldNames = _fieldNames;
}


void ncPutAttributeText(int ncid, int varid, const std::string &name, const std::string &value) {
    nc_put_att_text(ncid, varid, name.c_str(), value.size(), value.c_str());
}

bool VolumeData::writeToNcFile(const std::string& filePath) {
    int ncid = -1;
    int xVar{}, yVar{}, zVar{}, lonVar{}, latVar{};

    float xOrigin = 0.0f;
    float yOrigin = 0.0f;
    float zOrigin = 0.0f;

    int status = nc_create(filePath.c_str(), NC_NETCDF4 | NC_CLOBBER, &ncid);
    if (status != 0) {
        throw std::runtime_error(
                "Error in NetCdfWriter::writeFieldToFile: File \"" + filePath + "\" couldn't be opened.");
        return false;
    }

    ncPutAttributeText(ncid, NC_GLOBAL, "Conventions", "CF-1.5");
    ncPutAttributeText(ncid, NC_GLOBAL, "title", "Exported scalar field");
    ncPutAttributeText(ncid, NC_GLOBAL, "history", "ncconv");
    ncPutAttributeText(ncid, NC_GLOBAL, "institution", "Technical University of Munich, Chair of Computer Graphics and Visualization");
    ncPutAttributeText(ncid, NC_GLOBAL, "source", "ncconv, a utility program for converting meteorological data sets to the NetCDF format.");
    ncPutAttributeText(ncid, NC_GLOBAL, "references", "https://github.com/chrismile/ncconv");
    ncPutAttributeText(ncid, NC_GLOBAL, "comment", "ncconv is released under the 2-clause BSD license.");

    // Create dimensions.
    int xDim, yDim, zDim, tDim, eDim;
    nc_def_dim(ncid, "x", xs, &xDim);
    nc_def_dim(ncid, "y", ys, &yDim);
    nc_def_dim(ncid, "z", zs, &zDim);
    if (ts > 1) {
        nc_def_dim(ncid, "time", ts, &tDim);
    }
    if (es > 1) {
        nc_def_dim(ncid, "member", es, &eDim);
    }

    // Define the cell center variables.
    nc_def_var(ncid, "x", NC_FLOAT, 1, &xDim, &xVar);
    nc_def_var(ncid, "y", NC_FLOAT, 1, &yDim, &yVar);
    nc_def_var(ncid, "z", NC_FLOAT, 1, &zDim, &zVar);
    nc_def_var(ncid, "lon", NC_FLOAT, 1, &xDim, &lonVar);
    nc_def_var(ncid, "lat", NC_FLOAT, 1, &yDim, &latVar);

    ncPutAttributeText(ncid, xVar, "coordinate_type", "Cartesian X");
    ncPutAttributeText(ncid, yVar, "coordinate_type", "Cartesian Y");
    ncPutAttributeText(ncid, zVar, "coordinate_type", "Cartesian Z");
    // VTK interprets X, Y and Z as longitude, latitude and vertical.
    // Refer to VTK/IO/NetCDF/vtkNetCDFCFReader.cxx for more information.
    // ncPutAttributeText(ncid, xVar, "axis", "X");
    // ncPutAttributeText(ncid, yVar, "axis", "Y");
    // ncPutAttributeText(ncid, zVar, "axis", "Z");

    // Write the grid cell centers to the x, y and z variables.
    for (size_t x = 0; x < (size_t)xs; x++) {
        nc_put_var1_float(ncid, xVar, &x, lon1d + x);
        nc_put_var1_float(ncid, lonVar, &x, lon1d + x);
    }
    for (size_t y = 0; y < (size_t)ys; y++) {
        nc_put_var1_float(ncid, yVar, &y, lat1d + y);
        nc_put_var1_float(ncid, latVar, &y, lat1d + y);
    }
    for (size_t z = 0; z < (size_t)zs; z++) {
        nc_put_var1_float(ncid, zVar, &z, lev1d + z);
    }

    std::vector<int> dims;
    std::vector<size_t> start;
    std::vector<size_t> count;
    //int dimsTimeIndependent3D[] = { zDim, yDim, xDim };
    for (int varIdx = 0; varIdx < fieldNames.size(); varIdx++) {
        const std::string& fieldName = fieldNames.at(varIdx);
        std::cout << "Writing variable '" << fieldName << "'..." << std::endl;
        int varxs = 0, varys = 0, varzs = 0;
        float* fieldEntry = nullptr;
        volumeLoader->getFieldEntry(this, fieldName, 0, 0, fieldEntry, varxs, varys, varzs);
        varzs = std::max(varzs, 1);

        int zloc, yloc;
        if (ts <= 1 && varzs > 1) {
            dims = { zDim, yDim, xDim };
            zloc = 0;
            yloc = 1;
        } else if (ts > 1 && varzs > 1) {
            dims = { tDim, zDim, yDim, xDim };
            zloc = 1;
            yloc = 2;
        } else if (ts <= 1 && varzs <= 1) {
            dims = { yDim, xDim };
            zloc = -1;
            yloc = 0;
        } else if (ts > 1 && varzs <= 1) {
            dims = { tDim, yDim, xDim };
            zloc = -1;
            yloc = 1;
        }
        start.clear();
        count.clear();
        start.resize(dims.size(), 0);
        count.resize(dims.size(), 1);
        count.back() = size_t(xs);

        int scalarVar;
        nc_def_var(ncid, fieldName.c_str(), NC_FLOAT, int(dims.size()), dims.data(), &scalarVar);
        //size_t start[] = { 0, 0, 0 };
        //size_t count[] = { 1, 1, size_t(xs) };
        for (int t = 0; t < ts; t++) {
            if (t != 0) {
                delete[] fieldEntry;
                fieldEntry = nullptr;
                volumeLoader->getFieldEntry(this, fieldName, 0, 0, fieldEntry, varxs, varys, varzs);
                varzs = std::max(varzs, 1);
                start[0] = t;
            }
            for (int z = 0; z < varzs; z++) {
                if (zloc >= 0) {
                    start[zloc] = z;
                }
                for (int y = 0; y < varys; y++) {
                    start[yloc] = y;
                    nc_put_vara_float(
                            ncid, scalarVar, start.data(), count.data(), fieldEntry + y * varxs + z * varxs * varys);
                }
            }
        }
        delete[] fieldEntry;
    }

    if (nc_close(ncid) != NC_NOERR) {
        throw std::runtime_error(
                "Error in NetCdfWriter::writeFieldToFile: nc_close failed for file \"" + filePath + "\".");
        return false;
    }

    return true;
}
