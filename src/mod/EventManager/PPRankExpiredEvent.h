#pragma once

#include <ll/api/event/Event.h>

#include <mc/world/actor/player/Player.h>
#include <mc/world/lrvel/Level.h>

namespace ll::event::inline pp {
class PPRankExpiredEvent final : public Event {
    Level& mLevel;
    Player& mPlayer;
public:
    constexpr explicit ServerStoppingEvent(Level& level,Player& player) : mLevel(level),mPlayer(player) {}

    LLAPI void serialize(CompoundTag&) const override;

    LLNDAPI Level& level() const;
    LLNDAPI Player& player() const;
};
} // namespace ll::event::inline pp
