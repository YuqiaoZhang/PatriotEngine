/*
 * Copyright (C) YuqiaoZhang(HanetakaYuminaga)
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

#include <stddef.h>
#include <stdint.h>
#include <pt_mcrt_thread.h>
#include <assert.h>

#include "pt_wsi_window_posix_mach_objc.h"
#include "pt_wsi_window_posix_mach_foundation.h"
#include "pt_wsi_window_posix_mach_appkit.h"

class ns_application_delegate
{
public:
    static void application_did_finish_launching(NSApplicationDelegate, NSApplicationDelegate_applicationDidFinishLaunching_, void *aNotification);
    static void application_will_terminate(NSApplicationDelegate, NSApplicationDelegate_applicationWillTerminate_, void *aNotification);
    static int8_t application_should_terminate_after_last_window_closed(NSApplicationDelegate, NSApplicationDelegate_applicationShouldTerminateAfterLastWindowClosed_, NSApplication sender);
};

int main(int argc, char const *argv[])
{
    //Enable MultiThreaded
    {
        // Using Autorelease Pool Blocks
        // https://developer.apple.com/library/archive/documentation/Cocoa/Conceptual/MemoryMgmt/Articles/mmAutoreleasePools.html

        void *__here_auto_release_pool_object = objc_autoreleasePoolPush();

        bool __here_ns_thread_detach_target_has_finished = false;

        Class_NSThreadDetachTarget class_ns_thread_detach_target = NSThreadDetachTarget_allocateClass(
            "NSThreadDetachTarget_pt_wsi_window_posix_mach_osx",
            "NSMain_pt_wsi_window_posix_mach_osx",
            [](NSThreadDetachTarget, NSThreadDetachSelector_, void *__here_ns_thread_detach_target_has_finished) -> void {
                (*static_cast<bool *>(__here_ns_thread_detach_target_has_finished)) = true;
            });

        NSThreadDetachTarget ns_thread_detach_target = NSThreadDetachTarget_init(NSThreadDetachTarget_alloc(class_ns_thread_detach_target));

        NSThread_detachNewThreadSelector("NSMain_pt_wsi_window_posix_mach_osx", ns_thread_detach_target, &__here_ns_thread_detach_target_has_finished);

        while (!__here_ns_thread_detach_target_has_finished)
        {
            mcrt_os_yield();
        }

        NSThreadDetachTarget_release(ns_thread_detach_target);

        assert(NSThread_isMultiThreaded() != false);

        objc_autoreleasePoolPop(__here_auto_release_pool_object);
    }

    //Register NSApplicationDelegate
    {
        void *__here_auto_release_pool_object = objc_autoreleasePoolPush();

        Class_NSApplicationDelegate class_ns_application_delegate = NSApplicationDelegate_allocateClass(
            "NSApplicationDelegate_pt_wsi_window_posix_mach_osx",
            ns_application_delegate::application_did_finish_launching,
            ns_application_delegate::application_will_terminate,
            ns_application_delegate::application_should_terminate_after_last_window_closed);

        NSApplicationDelegate ns_application_delegate = NSApplicationDelegate_init(NSApplicationDelegate_alloc(class_ns_application_delegate));

        NSApplication ns_application = NSApplication_sharedApplication();

        NSApplication_setDelegate(ns_application, ns_application_delegate);

        objc_autoreleasePoolPop(__here_auto_release_pool_object);
    }

    return NSApplicationMain(argc, argv);
}

void ns_application_delegate::application_did_finish_launching(NSApplicationDelegate, NSApplicationDelegate_applicationDidFinishLaunching_, void *aNotification)
{
    NSSize ns_size_window = NSMakeSize(1280, 720);

    NSScreen ns_screen = NSScreen_mainScreen();

    NSSize ns_size_screen = NSScreen_frame(ns_screen).size;

    NSRect ns_rect = NSMakeRect((ns_size_screen.width - ns_size_window.width) / 2,
                                (ns_size_screen.height - ns_size_window.height) / 2,
                                ns_size_window.width,
                                ns_size_window.height);

    NSWindow ns_window = NSWindow_initWithContentRect(
        NSWindow_alloc(),
        ns_rect,
        NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable,
        NSBackingStoreBuffered,
        false,
        ns_screen);
}

void ns_application_delegate::application_will_terminate(NSApplicationDelegate, NSApplicationDelegate_applicationWillTerminate_, void *aNotification)
{
}

int8_t ns_application_delegate::application_should_terminate_after_last_window_closed(NSApplicationDelegate, NSApplicationDelegate_applicationShouldTerminateAfterLastWindowClosed_, NSApplication sender)
{
    return 1;
}

#include "pt_wsi_window_posix_mach_objc.inl"
#include "pt_wsi_window_posix_mach_foundation.inl"
#include "pt_wsi_window_posix_mach_appkit.inl"
