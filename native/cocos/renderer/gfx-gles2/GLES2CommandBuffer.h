/****************************************************************************
 Copyright (c) 2019-2023 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
****************************************************************************/

#pragma once

#include "GLES2Std.h"
#include "base/std/container/queue.h"
#include "gfx-base/GFXCommandBuffer.h"

namespace cc {
namespace gfx {

struct GLES2CmdPackage;
struct GLES2GPUPipelineState;
struct GLES2GPUDescriptorSet;
struct GLES2GPUInputAssembler;
class GLES2GPUCommandAllocator;

class CC_GLES2_API GLES2CommandBuffer : public CommandBuffer {
public:
    GLES2CommandBuffer();
    ~GLES2CommandBuffer() override;

    friend class GLES2Queue;

    void begin(RenderPass *renderPass, uint32_t subpass, Framebuffer *frameBuffer) override;
    void end() override;
    void beginRenderPass(RenderPass *renderPass, Framebuffer *fbo, const Rect &renderArea, const Color *colors, float depth, uint32_t stencil, CommandBuffer *const *secondaryCBs, uint32_t secondaryCBCount) override;
    void endRenderPass() override;
    void insertMarker(const MarkerInfo &marker) override;
    void beginMarker(const MarkerInfo &marker) override;
    void endMarker() override;
    void bindPipelineState(PipelineState *pso) override;
    void bindDescriptorSet(uint32_t set, DescriptorSet *descriptorSet, uint32_t dynamicOffsetCount, const uint32_t *dynamicOffsets) override;
    void bindInputAssembler(InputAssembler *ia) override;
    void setViewport(const Viewport &vp) override;
    void setScissor(const Rect &rect) override;
    void setLineWidth(float width) override;
    void setDepthBias(float constant, float clamp, float slope) override;
    void setBlendConstants(const Color &constants) override;
    void setDepthBound(float minBounds, float maxBounds) override;
    void setStencilWriteMask(StencilFace face, uint32_t mask) override;
    void setStencilCompareMask(StencilFace face, uint32_t ref, uint32_t mask) override;
    void nextSubpass() override;
    void draw(const DrawInfo &info) override;
    void drawIndirect(Buffer *buffer, uint32_t offset, uint32_t count, uint32_t stride) override;
    void drawIndexedIndirect(Buffer *buffer, uint32_t offset, uint32_t count, uint32_t stride) override;
    void updateBuffer(Buffer *buff, const void *data, uint32_t size) override;
    void copyBuffersToTexture(const uint8_t *const *buffers, Texture *texture, const BufferTextureCopy *regions, uint32_t count) override;
    void blitTexture(Texture *srcTexture, Texture *dstTexture, const TextureBlit *regions, uint32_t count, Filter filter) override;
    void resolveTexture(Texture *srcTexture, Texture *dstTexture, const TextureCopy *regions, uint32_t count) override;
    void copyTexture(Texture *srcTexture, Texture *dstTexture, const TextureCopy *regions, uint32_t count) override;
    void copyBuffer(Buffer *srcBuffer, Buffer *dstBuffer, const BufferCopy *regions, uint32_t count) override;
    void execute(CommandBuffer *const *cmdBuffs, uint32_t count) override;
    void dispatch(const DispatchInfo &info) override {}
    void pipelineBarrier(const GeneralBarrier *barrier, const BufferBarrier *const *bufferBarriers, const Buffer *const *buffers, uint32_t bufferCount, const TextureBarrier *const *textureBarriers, const Texture *const *textures, uint32_t textureBarrierCount) override {}
    void beginQuery(QueryPool *queryPool, uint32_t id) override {}
    void endQuery(QueryPool *queryPool, uint32_t id) override {}
    void resetQueryPool(QueryPool *queryPool) override {}

protected:
    void doInit(const CommandBufferInfo &info) override;
    void doDestroy() override;

    virtual void bindStates();

    GLES2GPUCommandAllocator *_cmdAllocator = nullptr;
    GLES2CmdPackage *_curCmdPackage = nullptr;
    ccstd::queue<GLES2CmdPackage *> _pendingPackages, _freePackages;

    uint32_t _curSubpassIdx = 0U;
    GLES2GPUPipelineState *_curGPUPipelineState = nullptr;
    ccstd::vector<GLES2GPUDescriptorSet *> _curGPUDescriptorSets;
    ccstd::vector<ccstd::vector<uint32_t>> _curDynamicOffsets;
    GLES2GPUInputAssembler *_curGPUInputAssember = nullptr;
    DynamicStates _curDynamicStates;

    bool _isStateInvalid = false;
};

} // namespace gfx
} // namespace cc
