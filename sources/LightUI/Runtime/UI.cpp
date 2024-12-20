#include "UI.h"

#include <format>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "LightGraphics/Runtime/SwapChain.h"
#include "LightWindow/Runtime/Input.h"
#include "LightWindow/Runtime/Window.h"

namespace Light
{
    void CheckVKResult(const VkResult err)
    {
        if (err >= 0)
            return;
        abort();
    }

    UI UI::Initialize(Window&, Graphics&)
    {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // IF using Docking Branch

        descriptorSetLayout = std::make_unique<GLDescriptorSetLayout>(std::vector<GLDescriptorBinding>{
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}
        });
        descriptorPool = std::make_unique<GLDescriptorPool>(*descriptorSetLayout, 10, VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);

        VkPipelineRenderingCreateInfo renderingInfo = {};
        renderingInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
        renderingInfo.pNext = VK_NULL_HANDLE;
        constexpr VkFormat colorFormat[] = {GraphicsPreset::DefaultColorFormat};
        renderingInfo.colorAttachmentCount = std::size(colorFormat);
        renderingInfo.pColorAttachmentFormats = colorFormat;
        renderingInfo.depthAttachmentFormat = GraphicsPreset::DefaultDepthStencilFormat;
        renderingInfo.stencilAttachmentFormat = GraphicsPreset::DefaultDepthStencilFormat;

        // Setup Platform/Renderer backends

        ImGui_ImplGlfw_InitForVulkan(Window::GetGlfwWindow(), true);
        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = GL::glInstance->instance;
        initInfo.PhysicalDevice = GL::glDevice->physicalDevice;
        initInfo.Device = GL::glDevice->device;
        initInfo.QueueFamily = GL::glDevice->graphicQueueFamily;
        initInfo.Queue = GL::glDevice->graphicQueue;
        initInfo.DescriptorPool = descriptorPool->descriptorPool; // See requirements in note above
        initInfo.MinImageCount = SwapChain::GetGLSwapChain().minImageCount; // >= 2
        initInfo.ImageCount = SwapChain::GetGLSwapChain().imageCount; // >= MinImageCount
        initInfo.MSAASamples = GraphicsPreset::DefaultStateLayout.multisample.rasterizationSamples; // 0 defaults to VK_SAMPLE_COUNT_1_BIT
        initInfo.UseDynamicRendering = true;
        initInfo.PipelineRenderingCreateInfo = renderingInfo;
        initInfo.CheckVkResultFn = CheckVKResult;
        ImGui_ImplVulkan_Init(&initInfo);
        ImGui_ImplVulkan_CreateFontsTexture();

        return {};
    }
    void UI::UnInitialize()
    {
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        descriptorPool.reset();
        descriptorSetLayout.reset();
    }
    void UI::BeginFrame()
    {
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    void UI::EndFrame(GLCommandBuffer& commandBuffer)
    {
        //生成绘制数据
        ImGui::Render();
        //提交绘制命令
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer.commandBuffer);
    }
    ImTextureID UI::CreateTexture(TextureAsset& texture)
    {
        const VkDescriptorSet descriptorSet = ImGui_ImplVulkan_AddTexture(
            GraphicsPreset::DefaultGLImageSampler->imageSampler,
            texture.glImageView->imageView,
            VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL);
        return descriptorSet;
    }
    void UI::DeleteTexture(const ImTextureID texture)
    {
        ImGui_ImplVulkan_RemoveTexture(static_cast<VkDescriptorSet>(texture));
    }
    float2 UI::GetWindowContentRegionSize()
    {
        const float2 windowMin = ImGui::GetWindowContentRegionMin();
        const float2 windowMax = ImGui::GetWindowContentRegionMax();
        return windowMax - windowMin;
    }
}
