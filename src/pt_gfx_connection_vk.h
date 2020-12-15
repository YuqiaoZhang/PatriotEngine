/*
 * Copyright (C) YuqiaoZhang
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef _PT_GFX_CONNECTION_VK_H_
#define _PT_GFX_CONNECTION_VK_H_ 1

#include <pt_gfx_connection.h>
#include "pt_gfx_connection_common.h"
#include <vulkan/vulkan.h>

class gfx_connection_vk : public gfx_connection_common
{
    VkAllocationCallbacks m_allocator_callbacks;

    VkInstance m_instance;
    VkPhysicalDevice m_physicalDevice;
    PFN_vkCreateInstance m_vkCreateInstance;
    VkPhysicalDevice m_physical_device;
    uint32_t m_queueGP_family_index;
    uint32_t m_queueT_family_index;
    VkQueue m_queueGP;
    VkQueue m_queueT;
#ifndef NDEBUG
    PFN_vkCreateDebugReportCallbackEXT m_vkCreateDebugReportCallbackEXT;
    PFN_vkDestroyDebugReportCallbackEXT m_vkDestroyDebugReportCallbackEXT;
#endif
    PFN_vkEnumeratePhysicalDevices m_vkEnumeratePhysicalDevices;
    PFN_vkGetPhysicalDeviceProperties m_vkGetPhysicalDeviceProperties;
    PFN_vkGetPhysicalDeviceQueueFamilyProperties m_vkGetPhysicalDeviceQueueFamilyProperties;
    PFN_vkGetPhysicalDeviceFormatProperties m_vkGetPhysicalDeviceFormatProperties;

    void *m_wsi_connection;
    void *m_visual;
    static char const *platform_surface_extension_name();
    bool platform_physical_device_presentation_support(VkPhysicalDevice physical_device, uint32_t queue_family_index);

#ifndef NDEBUG
    VkDebugReportCallbackEXT m_debug_report_callback;
    VkBool32 debug_report_callback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char *pLayerPrefix, const char *pMessage);
#endif

    struct gfx_itexture *create_texture() override;

    ~gfx_connection_vk();
    void destroy() override;

public:
    inline gfx_connection_vk() : m_wsi_connection(NULL), m_visual(reinterpret_cast<void *>(intptr_t(-1))) {}
    bool init();
    void size_change_callback(void *wsi_connection, void *visual, void *window, float width, float height);
    void draw_request_callback(void *wsi_connection, void *visual, void *window);
};

gfx_connection_vk *gfx_connection_vk_init(struct wsi_iwindow *window);

#endif