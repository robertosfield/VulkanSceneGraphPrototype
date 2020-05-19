#pragma once

/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/vk/ImageView.h>
#include <vsg/vk/RenderPass.h>
#include <vsg/vk/Framebuffer.h>

namespace vsg
{
    class VSG_DECLSPEC FrameAssembly : public Inherit<Object, FrameAssembly>
    {
    public:
        FrameAssembly()
        {
        }
        struct AttachmentImageView
        {
            ref_ptr<ImageView> imageView;
        };
        using AttachmentImageViews = std::vector<AttachmentImageView>;
        using ClearValues = std::vector<VkClearValue>;
        struct FrameRender
        {
            ref_ptr<Framebuffer> framebuffer;
            ref_ptr<RenderPass> renderPass;
            const ClearValues& clearValues;
            VkSampleCountFlagBits sampleBits;
        };
        // Not clear yet what arguments getFrameRender should pass.
        virtual FrameRender getFrameRender() = 0;
        virtual ref_ptr<Device> getDevice() const = 0;
        virtual const VkExtent2D& getExtent2D() const = 0;
    protected:
    };
}