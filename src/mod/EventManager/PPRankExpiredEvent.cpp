#include "PPRankExpiredEvent.h"
#include <ll/api/event/Emitter.h>

#include <mc/nbt/CompoundTag.h>

namespace ll::event::inline pp {

void PPRankExpiredEvent::serialize(CompoundTag& nbt) const {
    Event::serialize(nbt);
    nbt["world"] = (uintptr_t)&level();
    nbt["player"] = (uintptr_t)&player();
}

static std::unique_ptr<EmitterBase> emitterFactory(ListenerBase&);
class PPRankExpiredEventEmitter : public Emitter<emitterFactory, PPRankExpiredEvent> 
{
   std::cout << "Emit PPRankExpiredEvent\n";
};

static std::unique_ptr<EmitterBase> emitterFactory(ListenerBase&) {
    return std::make_unique<PPRankExpiredEventEmitter>();
}
}
/*LL_TYPE_INSTANCE_HOOK(
    ServerStoppingEventHook,
    HookPriority::Normal,
    ServerInstance,
    &ServerInstance::leaveGameSync,
    void
) {
    EventBus::getInstance().publish(ServerStoppingEvent(*this));
    origin();
}

static std::unique_ptr<EmitterBase> emitterFactory(ListenerBase&);
class ServerStoppingEventEmitter : public Emitter<emitterFactory, ServerStoppingEvent> {
    memory::HookRegistrar<ServerStoppingEventHook> hook;
};

static std::unique_ptr<EmitterBase> emitterFactory(ListenerBase&) {
    return std::make_unique<ServerStoppingEventEmitter>();
}*/
