/*
 * BSD 2-Clause License
 *
 * Copyright (c) 2022, Christoph Neuhauser
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

#ifndef CORRERENDER_LOADERSUTIL_HPP
#define CORRERENDER_LOADERSUTIL_HPP

#include <cstdint>

/**
 * Swaps the endianness of the passed array.
 * @param values The array to swap endianness for.
 * @param sizeInBytes The byte size of the array.
 * @param bytesPerEntry The size in bytes of one array entry. The bytes in each entry are shuffled.
 */
void swapEndianness(uint8_t* byteArray, size_t sizeInBytes, size_t bytesPerEntry);

/**
 * Swaps the endianness of the passed array.
 * @tparam T The data type. The bytes in each range of sizeof(T) are shuffled.
 * @param values The array to swap endianness for.
 * @param n The number of entries of byte size sizeof(T) in the array.
 */
template <typename T>
void swapEndianness(T* values, int n) {
    swapEndianness((uint8_t*)values, size_t(n) * sizeof(T), sizeof(T));
    /*auto* byteArray = (uint8_t*)values;
#ifdef USE_TBB
    tbb::parallel_for(tbb::blocked_range<int>(0, n), [&values, &byteArray](auto const& r) {
        for (auto i = r.begin(); i != r.end(); i++) {
#else
    #pragma omp parallel for shared(byteArray, values, n) default(none)
    for (int i = 0; i < n; i++) {
#endif
        uint8_t swappedEntry[sizeof(T)];
        for (size_t j = 0; j < sizeof(T); j++) {
            swappedEntry[j] = byteArray[i * sizeof(T) + sizeof(T) - j - 1];
        }
        values[i] = *((T*)&swappedEntry);
    }
#ifdef USE_TBB
    });
#endif*/
}

#endif //CORRERENDER_LOADERSUTIL_HPP
