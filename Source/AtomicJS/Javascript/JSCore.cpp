// Copyright (c) 2014-2015, THUNDERBEAST GAMES LLC All rights reserved
// Please see LICENSE.md in repository root for license information
// https://github.com/AtomicGameEngine/AtomicGameEngine

#include "JSCore.h"
#include "JSEventHelper.h"
#include "JSVM.h"

namespace Atomic
{

static int Object_SubscribeToEvent(duk_context* ctx)
{
    duk_push_this(ctx); // stack 2

    Object* object = js_to_class_instance<Object>(ctx, -1, 0);

    duk_get_prop_string(ctx, -1, "__eventHelper");

    if (duk_is_null_or_undefined(ctx, -1))
    {
        duk_pop(ctx); // pop null or undefined

        // construct a new event helper
        js_push_class_object_instance(ctx, new JSEventHelper(object->GetContext()));

        duk_push_object(ctx);
        duk_put_prop_string(ctx, -2, "__eventHelperFunctions");

        // dup so when we set the helper is left on stack
        duk_dup_top(ctx);

        duk_put_prop_string(ctx, 2, "__eventHelper");
    }

    JSEventHelper* helper = js_to_class_instance<JSEventHelper>(ctx, -1, 0);

    if (duk_is_string(ctx, 0))
    {
        StringHash eventType = duk_to_string(ctx, 0);

        duk_get_prop_string(ctx, -1, "__eventHelperFunctions");
        assert(duk_is_object(ctx, -1));
        assert(duk_is_function(ctx, 1));
        duk_dup(ctx, 1);
        duk_put_prop_string(ctx, -2,  eventType.ToString().CString());
        duk_pop(ctx);
        helper->AddEventHandler(eventType);
    }

    return 0;
}

void jsapi_init_core(JSVM* vm)
{
    duk_context* ctx = vm->GetJSContext();

    js_class_get_prototype(ctx, "AObject");
    duk_push_c_function(ctx, Object_SubscribeToEvent, 2);
    duk_put_prop_string(ctx, -2, "subscribeToEvent");

    duk_pop(ctx);
}

}
