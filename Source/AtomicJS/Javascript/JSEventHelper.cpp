
#include "JSVM.h"
#include "JSEventHelper.h"

namespace Atomic
{

JSEventHelper::JSEventHelper(Context* context) :
    Object(context)
{
}

JSEventHelper::~JSEventHelper()
{
    LOGINFO("Boom");
}

void JSEventHelper::AddEventHandler(StringHash eventType)
{
    SubscribeToEvent(eventType, HANDLER(JSEventHelper, HandleEvent));
}

void JSEventHelper::HandleEvent(StringHash eventType, VariantMap& eventData)
{
    JSVM* vm = JSVM::GetJSVM(context_);
    duk_context* ctx = vm->GetJSContext();

    duk_idx_t top = duk_get_top(ctx);

    js_push_class_object_instance(ctx, this);

    duk_get_prop_string(ctx, -1, "__eventHelperFunctions");

    assert(duk_is_object(ctx, -1));

    duk_get_prop_string(ctx, -1, eventType.ToString().CString());

    if (duk_is_function(ctx, -1))
    {
        if (duk_pcall(ctx, 0) != 0)
        {
            vm->SendJSErrorEvent();
        }
    }

    duk_set_top(ctx, top);

}

}
