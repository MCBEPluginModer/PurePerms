#include "PPRankExpiredEvent.h"
#include <ll/api/event/Emitter.h>

#include <mc/nbt/CompoundTag.h>
#include <iostream>
#include <ll/api/Logger.h>

namespace pp {
Level* PPRankExpiredEvent::level() const {
    return mLevel;
}

Player* PPRankExpiredEvent::player() const {
    return mPlayer;
}
void PPRankExpiredEvent::serialize(CompoundTag& nbt) const {
    ll::event::Cancellable<ll::event::Event>::serialize(nbt); 
    nbt["world"] = (uintptr_t)&level();
    nbt["player"] = (uintptr_t)&player();
}

void PPRankExpiredEvent::deserialize(CompoundTag const& nbt) {
    ll::event::Cancellable<ll::event::Event>::deserialize(nbt); // Вызов метода десериализации базового класса
    // Преобразование типов указателей
    level() = (Level*)nbt["world"];
    player() = (Player*)nbt["player"];
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
