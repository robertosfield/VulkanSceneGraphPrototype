/* <editor-fold desc="MIT License">

Copyright(c) 2018 Robert Osfield
Copyright(c) 2020 Julien Valentin

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</editor-fold> */

#include <vsg/vk/RenderPass.h>
#include <vsg/vk/PassGraph.h>
#include <array>

using namespace vsg;

RenderPass::RenderPass(VkRenderPass renderPass, Device* device, AllocationCallbacks* allocator) :
    _renderPass(renderPass),
    _device(device),
    _allocator(allocator)
{
}

RenderPass::~RenderPass()
{
    if (_renderPass)
    {
        vkDestroyRenderPass(*_device, _renderPass, _allocator);
    }
}

RenderPass::Result RenderPass::create(Device* device, VkFormat imageFormat, VkFormat depthFormat, AllocationCallbacks* allocator)
{
#if 0
    using Attachments = std::vector<VkAttachmentDescription>;
         using Subpasses = std::vector<VkSubpassDescription>;
         using Dependancies = std::vector<VkSubpassDependency>;
    Attachments attachments;

       VkAttachmentDescription colorAttachment = {};
       colorAttachment.format = imageFormat;
       colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
       colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
       colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
       colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
       colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
       colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
       colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
       attachments.push_back(colorAttachment);

       VkAttachmentDescription depthAttachment = {};
       depthAttachment.format = depthFormat;
       depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
       depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
       depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
       depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
       depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
       depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
       depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
       attachments.push_back(depthAttachment);

       VkAttachmentReference colorAttachmentRef = {};
       colorAttachmentRef.attachment = 0;
       colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

       VkAttachmentReference depthAttachmentRef = {};
       depthAttachmentRef.attachment = 1;
       depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

       Subpasses subpasses;

       VkSubpassDescription subpass = {};
       subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
       subpass.colorAttachmentCount = 1;
       subpass.pColorAttachments = &colorAttachmentRef;
       subpass.pDepthStencilAttachment = &depthAttachmentRef;
       subpasses.push_back(subpass);

       Dependancies dependancies;

       VkSubpassDependency dependency = {};
       dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
       dependency.dstSubpass = 0;
       dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
       dependency.srcAccessMask = 0;
       dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
       dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
       dependancies.push_back(dependency);
       VkRenderPassCreateInfo renderPassInfo = {};
           renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
           renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
           renderPassInfo.pAttachments = attachments.data();
           renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
           renderPassInfo.pSubpasses = subpasses.data();
           renderPassInfo.dependencyCount = static_cast<uint32_t>(dependancies.size());
           renderPassInfo.pDependencies = dependancies.data();

           VkRenderPass renderPass;
           VkResult result = vkCreateRenderPass(*device, &renderPassInfo, allocator, &renderPass);

           if (result == VK_SUCCESS)
           {
               return Result(new RenderPass(renderPass, device, allocator));
           }
           else
           {
               return Result("Error: vsg::RenderPass::create(...) Failed to create VkRenderPass.", result);
           }
#else
  if (!device)
    {
        return Result("Error: vsg::RenderPass::create(...) failed to create RenderPass, undefined Device.", VK_ERROR_INVALID_EXTERNAL_HANDLE);
    }
    ref_ptr<PassGraph> graph(PassGraph::create());
    //create  default render pass
    vsg::PassGraph::AttachmentDescription colorAttachment;

    colorAttachment.format = imageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    vsg::PassGraph::AttachmentDescription depthAttachment;
    depthAttachment.format = depthFormat;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    graph->addAttachmentDescription(colorAttachment);
    graph->addAttachmentDescription(depthAttachment);

    vsg::ref_ptr<vsg::SubPass > classicpass(graph->createSubPass());
    classicpass->addColorAttachmentRef(colorAttachment, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
    classicpass->addDepthStencilAttachmentRef(depthAttachment, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
    classicpass->setBindPoint(VK_PIPELINE_BIND_POINT_GRAPHICS);

    vsg::ref_ptr<vsg::Dependency > classicdep(classicpass->createForwardDependency());
    classicdep->setDstAccessMask(VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);

    return create(device, graph, allocator);
#endif
}

RenderPass::Result RenderPass::create(Device* device, PassGraph* graph, AllocationCallbacks* allocator)
{
    if (!device)
    {
        return Result("Error: vsg::RenderPass::create(...) failed to create RenderPass, undefined Device.", VK_ERROR_INVALID_EXTERNAL_HANDLE);
    }
    std::vector<VkSubpassDescription> subpasses;
    std::vector<VkSubpassDependency> dependencies;

    if(graph->getNumSubPasses()>0)
    {
        for(uint i=0; i<graph->getNumSubPasses(); ++i) subpasses.push_back(*graph->getSubPass(i));
        for(uint i=0; i<graph->getNumDependencies(); ++i) dependencies.push_back(*graph->getDependency(i));
    }

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount =  static_cast<uint32_t>(graph->getAttachmentDescriptions().size());
    renderPassInfo.pAttachments = graph->getAttachmentDescriptions().data();
    renderPassInfo.subpassCount =  static_cast<uint32_t>(subpasses.size());
    renderPassInfo.pSubpasses = subpasses.data();
    renderPassInfo.dependencyCount =  static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    VkRenderPass renderPass;
    VkResult result = vkCreateRenderPass(*device, &renderPassInfo, allocator, &renderPass);

    if (result == VK_SUCCESS)
    {
         Result res(new RenderPass(renderPass, device, allocator));

        res.object()->_passgraph = graph;
        return res;
    }
    else
    {
        return Result("Error: vsg::RenderPass::create(...) Failed to create VkRenderPass.", result);
    }

}
