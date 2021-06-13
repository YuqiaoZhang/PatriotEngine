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

static inline Class Class_NSObject_Unwrap(Class_NSObject class_ns_object)
{
    return reinterpret_cast<Class>(class_ns_object);
}

static inline NSObject NSObject_Wrap(struct objc_object *ns_object)
{
    return reinterpret_cast<NSObject>(ns_object);
}

static inline struct objc_object *NSObject_Unwrap(NSObject ns_object)
{
    return reinterpret_cast<struct objc_object *>(ns_object);
}

extern "C" void *objc_autoreleasePoolPush(void);
extern "C" void objc_autoreleasePoolPop(void *);

extern "C" PT_ATTR_APPLE_SDK void *AutoReleasePool_Push(void)
{
    return objc_autoreleasePoolPush();
}

extern "C" PT_ATTR_APPLE_SDK void AutoReleasePool_Pop(void *auto_release_pool_object)
{
    return objc_autoreleasePoolPop(auto_release_pool_object);
}

extern "C" PT_ATTR_APPLE_SDK bool Class_NSObject_addIvarVoidPointer(Class_NSObject class_ns_object, char const *ivarname)
{
    BOOL res = class_addIvar(
        Class_NSObject_Unwrap(class_ns_object),
        ivarname,
        sizeof(void *),
        alignof(void *),
        "^v");
    return (res != NO) ? true : false;
}

extern "C" PT_ATTR_APPLE_SDK NSObject NSObject_alloc(Class_NSObject class_ns_object)
{
    struct objc_object *ns_object = reinterpret_cast<struct objc_object *(*)(Class, struct objc_selector *)>(objc_msgSend)(
        Class_NSObject_Unwrap(class_ns_object),
        sel_registerName("alloc"));
    return NSObject_Wrap(ns_object);
}

extern "C" PT_ATTR_APPLE_SDK NSObject NSObject_init(NSObject ns_object)
{
    struct objc_object *ret_ns_object = reinterpret_cast<struct objc_object *(*)(struct objc_object *, struct objc_selector *)>(objc_msgSend)(
        NSObject_Unwrap(ns_object),
        sel_registerName("init"));
    return NSObject_Wrap(ret_ns_object);
}

extern "C" PT_ATTR_APPLE_SDK void NSObject_release(NSObject ns_object)
{
    return reinterpret_cast<void (*)(struct objc_object *, struct objc_selector *)>(objc_msgSend)(
        NSObject_Unwrap(ns_object),
        sel_registerName("release"));
}