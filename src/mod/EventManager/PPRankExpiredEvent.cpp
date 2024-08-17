#include "PPRankExpiredEvent.h"
#include <ll/api/event/Emitter.h>

#include <mc/nbt/CompoundTag.h>
#include <iostream>
#include <ll/api/Logger.h>

namespace pp {
void PPRankExpiredEvent::serialize(CompoundTag& nbt) const {
    ll::event::Cancellable<RankEvent>::serialize(nbt); 
    nbt["world"] = (uintptr_t)&level();
    nbt["player"] = (uintptr_t)&player();
}

void PPRankExpiredEvent::deserialize(CompoundTag const& nbt) {
    ll::event::Cancellable<RankEvent>::deserialize(nbt); // Вызов метода десериализации базового класса
    // Преобразование типов указателей
    level() = *reinterpret_cast<Level*>(nbt["world"].getInt64());
    player() = *reinterpret_cast<Player*>(nbt["player"].getInt64());
}

Level& RankEvent::level() const {
    return mLevel;
}

Player& RankEvent::player() const {
    return mPlayer;
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
