#pragma once

#include <ll/api/event/Event.h>

#include <mc/world/actor/player/Player.h>
#include <mc/world/level/Level.h>
#include <ll/api/event/Cancellable.h>
#include "../PPGroup.h"
#include <iostream>

/*#ifdef _WIN32
#   ifdef BUILDING_DLL
#       define LLAPI __declspec(dllexport)
#       define LLNDAPI __declspec(dllexport)
#   else
#       define LLAPI __declspec(dllimport)
#       define LLNDAPI __declspec(dllimport)
#   endif
#else
#   define LLAPI
#   define LLNDAPI
#endif*/

namespace pp {

class PPRankExpiredEvent final : public ll::event::Cancellable<ll::event::Event>
{
    Level& mLevel;
    Player& mPlayer;
public:
    constexpr explicit PPRankExpiredEvent(Level& level,Player& player) : mLevel(level),mPlayer(player), Cancellable() {}
    LLNDAPI Level& level() const;
    LLNDAPI Player& player() const;
};

class PPRankChangedEvent  final : public ll::event::Cancellable<ll::event::Event>
{
    Level& mLevel;
    Player& mPlayer;
    PPGroup& mGroup;
public:
    constexpr explicit PPRankChangedEvent (Level& level,Player& player,PPGroup& group) : mLevel(level),mPlayer(player),mGroup(group), Cancellable() {}
    LLNDAPI Level& level() const;
    LLNDAPI Player& player() const;
    LLNDAPI PPGroup& group() const;
};
} // namespace ll::event::inline pp
