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

#include <pt_apple_sdk_posix_mach_objc.h>
#include <pt_apple_sdk_posix_mach_appkit.h>

#include <TargetConditionals.h>
#if TARGET_OS_IOS
#include <objc/message.h>
#include <objc/runtime.h>
#elif TARGET_OS_OSX
#include <objc/objc-runtime.h>
#else
#error Unknown Target
#endif

#include <assert.h>

static inline Class_NSApplicationDelegate Class_NSApplicationDelegate_Wrap(Class class_ns_application_delegate)
{
    return reinterpret_cast<Class_NSApplicationDelegate>(class_ns_application_delegate);
}

static inline Class Class_NSApplicationDelegate_Unwrap(Class_NSApplicationDelegate class_ns_application_delegate)
{
    return reinterpret_cast<Class>(class_ns_application_delegate);
}

static inline NSObject NSApplicationDelegate_To_NSObject(NSApplicationDelegate ns_application_delegate)
{
    return reinterpret_cast<NSObject>(ns_application_delegate);
}

static inline NSApplicationDelegate NSObject_To_NSApplicationDelegate(NSObject ns_application_delegate)
{
    return reinterpret_cast<NSApplicationDelegate>(ns_application_delegate);
}

static inline NSApplicationDelegate NSApplicationDelegate_Wrap(struct objc_object *ns_application_delegate)
{
    return reinterpret_cast<NSApplicationDelegate>(ns_application_delegate);
}

static inline struct objc_object *NSApplicationDelegate_Unwrap(NSApplicationDelegate ns_application_delegate)
{
    return reinterpret_cast<struct objc_object *>(ns_application_delegate);
}

static inline NSApplication NSApplication_Wrap(struct objc_object *ns_application)
{
    return reinterpret_cast<NSApplication>(ns_application);
}

static inline struct objc_object *NSApplication_Unwrap(NSApplication ns_application)
{
    return reinterpret_cast<struct objc_object *>(ns_application);
}

static inline NSScreen NSScreen_Wrap(struct objc_object *ns_screen)
{
    return reinterpret_cast<NSScreen>(ns_screen);
}

static inline struct objc_object *NSScreen_Unwrap(NSScreen ns_screen)
{
    return reinterpret_cast<struct objc_object *>(ns_screen);
}

static inline Class_NSObject Class_NSWindow_To_Class_NSObject(Class_NSWindow class_ns_window)
{
    return reinterpret_cast<Class_NSObject>(class_ns_window);
}

static inline NSWindow NSWindow_Wrap(struct objc_object *ns_window)
{
    return reinterpret_cast<NSWindow>(ns_window);
}

static inline struct objc_object *NSWindow_Unwrap(NSWindow ns_window)
{
    return reinterpret_cast<struct objc_object *>(ns_window);
}

static inline NSWindow NSObject_To_NSWindow(NSObject ns_window)
{
    return reinterpret_cast<NSWindow>(ns_window);
}

static inline Class_NSView Class_NSView_Wrap(Class class_ns_view)
{
    return reinterpret_cast<Class_NSView>(class_ns_view);
}

static inline Class Class_NSView_Unwrap(Class_NSView class_ns_view)
{
    return reinterpret_cast<Class>(class_ns_view);
}

static inline NSView NSView_Wrap(struct objc_object *ns_view)
{
    return reinterpret_cast<NSView>(ns_view);
}

static inline struct objc_object *NSView_Unwrap(NSView ns_view)
{
    return reinterpret_cast<struct objc_object *>(ns_view);
}

static inline CALayer CALayer_Wrap(struct objc_object *ca_layer)
{
    return reinterpret_cast<CALayer>(ca_layer);
}

static inline Class_NSViewController Class_NSViewController_Wrap(Class class_ns_view_controller)
{
    return reinterpret_cast<Class_NSViewController>(class_ns_view_controller);
}

static inline Class Class_NSViewController_Unwrap(Class_NSViewController class_ns_view_controller)
{
    return reinterpret_cast<Class>(class_ns_view_controller);
}

static inline Class_NSObject Class_NSViewController_To_Class_NSObject(Class_NSViewController class_ns_view_controller)
{
    return reinterpret_cast<Class_NSObject>(class_ns_view_controller);
}

static inline NSViewController NSViewController_Wrap(struct objc_object *ns_view_controller)
{
    return reinterpret_cast<NSViewController>(ns_view_controller);
}

static inline struct objc_object *NSViewController_Unwrap(NSViewController ns_view_controller)
{
    return reinterpret_cast<struct objc_object *>(ns_view_controller);
}

static inline NSObject NSViewController_To_NSObject(NSViewController ns_view_controller)
{
    return reinterpret_cast<NSObject>(ns_view_controller);
}

// ---
PT_ATTR_APPLE_SDK Class_NSApplicationDelegate NSApplicationDelegate_allocateClass(
    char const *class_name,
    void (*_I_NSApplicationDelegate_applicationDidFinishLaunching_)(NSApplicationDelegate, NSApplicationDelegate_applicationDidFinishLaunching_, void *aNotification),
    void (*_I_NSApplicationDelegate_applicationWillTerminate_)(NSApplicationDelegate, NSApplicationDelegate_applicationWillTerminate_, void *aNotification),
    int8_t (*_I_NSApplicationDelegate_applicationShouldTerminateAfterLastWindowClosed_)(NSApplicationDelegate, NSApplicationDelegate_applicationShouldTerminateAfterLastWindowClosed_, NSApplication sender))
{
    //objc_object *protocol_NSApplicationDelegate = objc_getProtocol("NSApplicationDelegate");
    //return NULL why???

    Class class_ns_application_delegate = objc_allocateClassPair(
        objc_getClass("NSObject"),
        class_name,
        0);
    assert(class_ns_application_delegate != NULL);

    BOOL result_did_finish_launching = class_addMethod(
        class_ns_application_delegate,
        sel_registerName("applicationDidFinishLaunching:"),
        reinterpret_cast<IMP>(_I_NSApplicationDelegate_applicationDidFinishLaunching_),
        "v@:@");
    assert(result_did_finish_launching != NO);

    BOOL result_will_terminate = class_addMethod(
        class_ns_application_delegate,
        sel_registerName("applicationWillTerminate:"),
        reinterpret_cast<IMP>(_I_NSApplicationDelegate_applicationWillTerminate_),
        "v@:@");
    assert(result_will_terminate != NO);

    BOOL result_should_terminate_after_last_window_closed = class_addMethod(
        class_ns_application_delegate,
        sel_registerName("applicationShouldTerminateAfterLastWindowClosed:"),
        reinterpret_cast<IMP>(_I_NSApplicationDelegate_applicationShouldTerminateAfterLastWindowClosed_),
        "v@:@");
    assert(result_should_terminate_after_last_window_closed != NO);

    //res = class_addProtocol(
    //      class_MyDelegate,
    //      protocol_NSApplicationDelegate);

    //assert(res != NO);

    //objc_registerClassPair(class_NSApplicationDelegate_CXX);

    //res = class_conformsToProtocol(class_NSApplicationDelegate_CXX, protocol_NSApplicationDelegate);
    //assert(res != NO);

    return Class_NSApplicationDelegate_Wrap(class_ns_application_delegate);
}

PT_ATTR_APPLE_SDK void Class_NSApplicationDelegate_register(Class_NSApplicationDelegate class_ns_application_delegate)
{
    return objc_registerClassPair(Class_NSApplicationDelegate_Unwrap(class_ns_application_delegate));
}

PT_ATTR_APPLE_SDK NSApplicationDelegate NSApplicationDelegate_alloc(Class_NSApplicationDelegate class_ns_application_delegate)
{
    struct objc_object *ns_application_delegate = reinterpret_cast<struct objc_object *(*)(Class, struct objc_selector *)>(objc_msgSend)(
        Class_NSApplicationDelegate_Unwrap(class_ns_application_delegate),
        sel_registerName("alloc"));
    return NSApplicationDelegate_Wrap(ns_application_delegate);
}

PT_ATTR_APPLE_SDK NSApplicationDelegate NSApplicationDelegate_init(NSApplicationDelegate ns_application_delegate)
{
    return NSObject_To_NSApplicationDelegate(NSObject_init(NSApplicationDelegate_To_NSObject(ns_application_delegate)));
}

PT_ATTR_APPLE_SDK NSApplication NSApplication_sharedApplication()
{
    struct objc_object *ns_application = reinterpret_cast<struct objc_object *(*)(Class, struct objc_selector *)>(objc_msgSend)(
        objc_getClass("NSApplication"),
        sel_registerName("sharedApplication"));

    return NSApplication_Wrap(ns_application);
}

PT_ATTR_APPLE_SDK void NSApplication_setDelegate(NSApplication ns_application, NSApplicationDelegate ns_application_delegate)
{
    return reinterpret_cast<void (*)(struct objc_object *, struct objc_selector *, struct objc_object *)>(objc_msgSend)(
        NSApplication_Unwrap(ns_application),
        sel_registerName("setDelegate:"),
        NSApplicationDelegate_Unwrap(ns_application_delegate));
}

extern "C" int NSApplicationMain(int argc, char const *argv[]);

PT_ATTR_APPLE_SDK int NSApplication_Main(int argc, char const *argv[])
{
    return NSApplicationMain(argc, argv);
}

PT_ATTR_APPLE_SDK NSScreen NSScreen_mainScreen()
{
    struct objc_object *ns_screen = reinterpret_cast<struct objc_object *(*)(Class, struct objc_selector *)>(objc_msgSend)(
        objc_getClass("NSScreen"),
        sel_registerName("mainScreen"));

    return NSScreen_Wrap(ns_screen);
}

PT_ATTR_APPLE_SDK NSRect NSScreen_frame(NSScreen ns_screen)
{
#if defined(__x86_64__) || defined(__i386__) || defined(__arm__)
    assert(sizeof(NSRect) != 1 && sizeof(NSRect) != 2 && sizeof(NSRect) != 4 && sizeof(NSRect) != 8);
    NSRect ns_rect = reinterpret_cast<NSRect (*)(struct objc_object *, struct objc_selector *)>(objc_msgSend_stret)(
        NSScreen_Unwrap(ns_screen),
        sel_registerName("frame"));
#elif defined(__aarch64__)
    assert(sizeof(NSRect) != 1 && sizeof(NSRect) != 2 && sizeof(NSRect) != 4 && sizeof(NSRect) != 8);
    NSRect ns_rect = reinterpret_cast<NSRect (*)(struct objc_object *, struct objc_selector *)>(objc_msgSend)(
        NSScreen_Unwrap(ns_screen),
        sel_registerName("frame"));
#else
#error Unknown Architecture
#endif
    return ns_rect;
}

PT_ATTR_APPLE_SDK NSWindow NSWindow_alloc()
{
    Class_NSWindow class_ns_window = reinterpret_cast<Class_NSWindow>(objc_getClass("NSWindow"));
    return NSObject_To_NSWindow(NSObject_alloc(Class_NSWindow_To_Class_NSObject(class_ns_window)));
}

PT_ATTR_APPLE_SDK NSWindow NSWindow_initWithContentRect(NSWindow ns_window, NSRect ns_rect, NSWindowStyleMask ns_window_style_mask, NSBackingStoreType ns_backing_store_type, bool flag, NSScreen ns_screen)
{
    struct objc_object *ret_ns_window = reinterpret_cast<struct objc_object *(*)(struct objc_object *, struct objc_selector *, NSRect, NSWindowStyleMask, NSBackingStoreType, BOOL, struct objc_object *)>(objc_msgSend)(
        NSWindow_Unwrap(ns_window),
        sel_registerName("initWithContentRect:styleMask:backing:defer:screen:"),
        ns_rect,
        ns_window_style_mask,
        ns_backing_store_type,
        (flag != false) ? YES : NO,
        NSScreen_Unwrap(ns_screen));
    return NSWindow_Wrap(ret_ns_window);
}

PT_ATTR_APPLE_SDK void NSWindow_setContentViewController(NSWindow ns_window, NSViewController ns_view_controller)
{
    return reinterpret_cast<void (*)(struct objc_object *, struct objc_selector *, struct objc_object *)>(objc_msgSend)(
        NSWindow_Unwrap(ns_window),
        sel_registerName("setContentViewController:"),
        NSViewController_Unwrap(ns_view_controller));
}

PT_ATTR_APPLE_SDK void NSWindow_makeKeyAndOrderFront(NSWindow ns_window, void *sender)
{
    return reinterpret_cast<void (*)(struct objc_object *, struct objc_selector *, struct objc_object *)>(objc_msgSend)(
        NSWindow_Unwrap(ns_window),
        sel_registerName("makeKeyAndOrderFront:"),
        reinterpret_cast<struct objc_object *>(sender));
}

//static Class _PT_C_UIView_layerClass(Class class_ui_view, struct objc_selector *)
//{
//    return objc_getClass("CAMetalLayer");
//}

static signed char _PT_I_NSView_wantsLayer(NSView ns_view, NSView_wantsLayer)
{
    return YES;
}

static void *_PT_I_NSView_makeBackingLayer(NSView ns_view, NSView_makeBackingLayer)
{
    struct objc_object *ret_ca_layer = reinterpret_cast<struct objc_object *(*)(Class, struct objc_selector *)>(objc_msgSend)(
        objc_getClass("CAMetalLayer"),
        sel_registerName("layer"));
    return ret_ca_layer;
}

static signed char _PT_I_NSView_wantsUpdateLayer(NSView ns_view, NSView_wantsUpdateLayer)
{
    return YES;
}

PT_ATTR_APPLE_SDK Class_NSView NSView_allocateClass(
    char const *class_name,
    signed char (*_I_NSView_wantsLayer)(NSView ns_view, NSView_wantsLayer),
    void *(*_I_NSView_makeBackingLayer)(NSView ns_view, NSView_makeBackingLayer),
    signed char (*_I_NSView_wantsUpdateLayer)(NSView ns_view, NSView_wantsUpdateLayer))
{
    Class class_ns_view = objc_allocateClassPair(
        objc_getClass("NSView"),
        class_name,
        0);
    assert(class_ns_view != NULL);

    //Class metaclass_ns_view = objc_getMetaClass(class_name);
    //BOOL result_layer_class = class_addMethod(
    //    metaclass_ns_view,
    //    sel_registerName("layerClass"),
    //    reinterpret_cast<IMP>(_PT_C_NSView_layerClass),
    //    "#@:");
    //assert(result_layer_class != NO);

    // [Creating a Custom Metal View](https://developer.apple.com/documentation/metal/drawable_objects/creating_a_custom_metal_view)

    BOOL result_wants_layer = class_addMethod(
        class_ns_view,
        sel_registerName("wantsLayer"),
        reinterpret_cast<IMP>((NULL == _I_NSView_wantsLayer) ? _PT_I_NSView_wantsLayer : _I_NSView_wantsLayer),
        "c@:");
    assert(result_wants_layer != NO);

    BOOL result_make_backing_layer = class_addMethod(
        class_ns_view,
        sel_registerName("makeBackingLayer"),
        reinterpret_cast<IMP>((NULL == _I_NSView_makeBackingLayer) ? _PT_I_NSView_makeBackingLayer : _I_NSView_makeBackingLayer),
        "@@:");
    assert(result_make_backing_layer != NO);

    BOOL result_wants_update_layer = class_addMethod(
        class_ns_view,
        sel_registerName("wantsUpdateLayer"),
        reinterpret_cast<IMP>((NULL == _I_NSView_wantsUpdateLayer) ? _PT_I_NSView_wantsUpdateLayer : _I_NSView_wantsUpdateLayer),
        "c@:");
    assert(result_wants_update_layer != NO);

    return Class_NSView_Wrap(class_ns_view);
}

PT_ATTR_APPLE_SDK void Class_NSView_register(Class_NSView class_ns_view)
{
    // The class seems incomplete without being registered and the MoltenVK would crash
    return objc_registerClassPair(Class_NSView_Unwrap(class_ns_view));
}

PT_ATTR_APPLE_SDK NSView NSView_alloc(Class_NSView class_ns_view)
{
    struct objc_object *ns_view = reinterpret_cast<struct objc_object *(*)(Class, struct objc_selector *)>(objc_msgSend)(
        Class_NSView_Unwrap(class_ns_view),
        sel_registerName("alloc"));
    return NSView_Wrap(ns_view);
}

PT_ATTR_APPLE_SDK NSView NSView_initWithFrame(NSView ns_view, NSRect frame_rect)
{
    struct objc_object *ret_ns_view = reinterpret_cast<struct objc_object *(*)(struct objc_object *, struct objc_selector *, NSRect)>(objc_msgSend)(
        NSView_Unwrap(ns_view),
        sel_registerName("initWithFrame:"),
        frame_rect);
    return NSView_Wrap(ret_ns_view);
}

PT_ATTR_APPLE_SDK CALayer NSView_layer(NSView ns_view)
{
    struct objc_object *ret_ca_layer = reinterpret_cast<struct objc_object *(*)(struct objc_object *, struct objc_selector *)>(objc_msgSend)(
        NSView_Unwrap(ns_view),
        sel_registerName("layer"));
    return CALayer_Wrap(ret_ca_layer);
}

PT_ATTR_APPLE_SDK Class_NSViewController NSViewController_allocateClass(
    char const *class_name,
    void (*_I_NSViewController_loadView)(NSViewController ns_view_controller, NSViewController_loadView),
    void (*_I_NSViewController_viewDidLoad)(NSViewController ns_view_controller, NSViewController_viewDidLoad),
    void (*_I_NSViewController_setRepresentedObject_)(NSViewController ns_view_controller, NSViewController_setRepresentedObject_, void *represented_object))
{

    Class class_ns_view_controller = objc_allocateClassPair(
        objc_getClass("NSViewController"),
        class_name,
        0);
    assert(class_ns_view_controller != NULL);

    BOOL result_load_view = class_addMethod(
        class_ns_view_controller,
        sel_registerName("loadView"),
        reinterpret_cast<IMP>(_I_NSViewController_loadView),
        "v@:");
    assert(result_load_view != NO);

    BOOL result_view_did_load = class_addMethod(
        class_ns_view_controller,
        sel_registerName("viewDidLoad"),
        reinterpret_cast<IMP>(_I_NSViewController_viewDidLoad),
        "v@:");
    assert(result_view_did_load != NO);

    BOOL result_set_represented_object = class_addMethod(
        class_ns_view_controller,
        sel_registerName("setRepresentedObject:"),
        reinterpret_cast<IMP>(_I_NSViewController_setRepresentedObject_),
        "v@:@");
    assert(result_set_represented_object != NO);

    return Class_NSViewController_Wrap(class_ns_view_controller);
}

PT_ATTR_APPLE_SDK bool Class_NSViewController_addIvarVoidPointer(Class_NSViewController class_ns_view_controller, char const *ivarname)
{
    return Class_NSObject_addIvarVoidPointer(Class_NSViewController_To_Class_NSObject(class_ns_view_controller), ivarname);
}

PT_ATTR_APPLE_SDK void Class_NSViewController_register(Class_NSViewController class_ns_view_controller)
{
    return objc_registerClassPair(Class_NSViewController_Unwrap(class_ns_view_controller));
}

PT_ATTR_APPLE_SDK NSViewController NSViewController_alloc(Class_NSViewController class_ns_view_controller)
{
    struct objc_object *ns_view_controller = reinterpret_cast<struct objc_object *(*)(Class, struct objc_selector *)>(objc_msgSend)(
        Class_NSViewController_Unwrap(class_ns_view_controller),
        sel_registerName("alloc"));
    return NSViewController_Wrap(ns_view_controller);
}

PT_ATTR_APPLE_SDK NSViewController NSViewController_initWithNibName(NSViewController ns_view_controller, void *nibNameOrNil, void *nibBundleOrNil)
{

    struct objc_object *ret_ns_view_controller = reinterpret_cast<struct objc_object *(*)(struct objc_object *, struct objc_selector *, struct objc_object *, struct objc_object *)>(objc_msgSend)(
        NSViewController_Unwrap(ns_view_controller),
        sel_registerName("initWithNibName:bundle:"),
        reinterpret_cast<struct objc_object *>(nibNameOrNil),
        reinterpret_cast<struct objc_object *>(nibBundleOrNil));
    return NSViewController_Wrap(ret_ns_view_controller);
}

PT_ATTR_APPLE_SDK void NSViewController_setView(NSViewController ns_view_controller, NSView ns_view)
{
    return reinterpret_cast<void (*)(struct objc_object *, struct objc_selector *, struct objc_object *)>(objc_msgSend)(
        NSViewController_Unwrap(ns_view_controller),
        sel_registerName("setView:"),
        NSView_Unwrap(ns_view));
}

PT_ATTR_APPLE_SDK void NSViewController_super_viewDidLoad(NSViewController ns_view_controller, NSViewController_viewDidLoad cmd)
{
    struct objc_super super = {
        NSViewController_Unwrap(ns_view_controller),
        class_getSuperclass(object_getClass(NSViewController_Unwrap(ns_view_controller)))};

    return reinterpret_cast<void (*)(struct objc_super *, struct objc_selector *)>(objc_msgSendSuper)(
        &super,
        reinterpret_cast<struct objc_selector *>(cmd));
}

PT_ATTR_APPLE_SDK void NSViewController_super_setRepresentedObject_(NSViewController ns_view_controller, NSViewController_setRepresentedObject_ cmd, void *represented_object)
{
    struct objc_super super = {
        NSViewController_Unwrap(ns_view_controller),
        class_getSuperclass(object_getClass(NSViewController_Unwrap(ns_view_controller)))};

    return reinterpret_cast<void (*)(struct objc_super *, struct objc_selector *, struct objc_object *)>(objc_msgSendSuper)(
        &super,
        reinterpret_cast<struct objc_selector *>(cmd),
        reinterpret_cast<struct objc_object *>(represented_object));
}

PT_ATTR_APPLE_SDK void NSViewController_setIvarVoidPointer(NSViewController ns_view_controller, char const *ivarname, void *pVoid)
{
    return NSObject_setIvarVoidPointer(NSViewController_To_NSObject(ns_view_controller), ivarname, pVoid);
}

PT_ATTR_APPLE_SDK void *NSViewController_getIvarVoidPointer(NSViewController ns_view_controller, char const *ivarname)
{
    return NSObject_getIvarVoidPointer(NSViewController_To_NSObject(ns_view_controller), ivarname);
}
