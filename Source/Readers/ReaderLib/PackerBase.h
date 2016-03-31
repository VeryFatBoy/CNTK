//
// Copyright (c) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE.md file in the project root for full license information.
//

#pragma once

#include "Reader.h"
#include "MemoryProvider.h"
#include "Transformer.h"
#include "Packer.h"
#include <deque>

namespace Microsoft { namespace MSR { namespace CNTK {

// A base class for Packers.
class PackerBase : public Packer
{
public:

    virtual Minibatch ReadMinibatch() override;

protected:

    struct StreamBuffer
    {
        size_t m_size; // buffer size in bytes.
        // Memory provider.
        // TODO: Should possibly switch to matrices here.
        MemoryProviderPtr m_memoryProvider;
        std::shared_ptr<char> m_data; // contiguous array of data.

        StreamBuffer(MemoryProviderPtr m_memoryProvider) :
            m_size(0), m_memoryProvider(m_memoryProvider), m_data(nullptr)
        {
        }
        void Resize(size_t newSize);
    };

    PackerBase(MemoryProviderPtr memoryProvider,
        TransformerPtr transformer,
        size_t minibatchSize,
        const std::vector<StreamDescriptionPtr>& streams);

    typedef std::vector<SequenceDataPtr> StreamBatch;

    size_t GetSampleSize(StreamDescriptionPtr stream);

    virtual MBLayoutPtr PackDenseStream(const StreamBatch& batch, size_t streamIndex);

    virtual MBLayoutPtr PackSparseStream(const StreamBatch& batch, size_t streamIndex);
   
    virtual MBLayoutPtr CreateMBLayout(const StreamBatch& batch) = 0;
    
    TransformerPtr m_transformer;

    // Input stream descriptions provided by the transformer.
    std::vector<StreamDescriptionPtr> m_outputStreamDescriptions;

    // Output stream descriptions expected by the network.
    std::vector<StreamDescriptionPtr> m_inputStreamDescriptions;

    std::vector<StreamBuffer> m_streamBuffers;

    // Minibatch size in samples.
    size_t m_minibatchSize;
};

}}}