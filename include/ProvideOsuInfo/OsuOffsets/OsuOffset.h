#pragma once
#include <cstdint>
#include <vector>

/*
@brief
    Offsets for osu! game elements and structures.
*/

namespace OsuOffsets {
    inline constexpr uint32_t OsuGame_osuLogo = 1584;
    inline constexpr uint32_t OsuGame_ScreenStack = 1544;
    inline constexpr uint32_t OsuGame_frameworkConfig = 1480;
    inline constexpr uint32_t OsuGame_channelManager = 1320;
    inline constexpr uint32_t OsuGame_chatOverlay = 1312;

    inline constexpr uint32_t Game_Host = 824;

    inline constexpr uint32_t OsuGameBase_API = 1080;
    inline constexpr uint32_t OsuGameBase_SpectatorClient = 1192;
    inline constexpr uint32_t OsuGameBase_ScoreManager = 1024;
    inline constexpr uint32_t OsuGameBase_MultiplayerClient = 1200;
    inline constexpr uint32_t OsuGameBase_LeaderboardManager = 1168;
    inline constexpr uint32_t OsuGameBase_SessionStatics = 992;
    inline constexpr uint32_t OsuGameBase_beatmapClock = 1232;
    inline constexpr uint32_t OsuGameBase_Storage = 1088;
    inline constexpr uint32_t OsuGameBase_Beatmap = 1104;
    inline constexpr uint32_t OsuGameBase_SkinManager = 1040;
    inline constexpr uint32_t OsuGameBase_VersionHash = 976;
    inline constexpr uint32_t OsuGameBase_LocalConfig = 984;
    inline constexpr uint32_t OsuGameBase_rulesetConfigCache = 1176;
    inline constexpr uint32_t OsuGameBase_realm = 1216;

    inline constexpr uint32_t ScreenStack_stack = 800;

    inline constexpr uint32_t Player_api = 1008;
    inline constexpr uint32_t Player_scoreManager = 1000;
    inline constexpr uint32_t Player_Score = 1152;
    inline constexpr uint32_t Player_ScoreProcessor = 1096;
    inline constexpr uint32_t Player_HealthProcessor = 1104;
    inline constexpr uint32_t Player_HUDOverlay = 1120;
    inline constexpr uint32_t Player_DrawableRuleset = 1112;
    inline constexpr uint32_t Player_dependencies = 1168;

    inline constexpr uint32_t OsuScoreProcessor_Combo = 592;

    inline constexpr uint32_t ExternalLinkOpener_api = 536;

    inline constexpr uint32_t APIAccess_game = 784;
    inline constexpr uint32_t APIAccess_localUserState = 864;

    inline constexpr uint32_t OsuGameDesktop_AvailableMods = 1128;
    inline constexpr uint32_t OsuGameDesktop_SelectedMods = 1120;
    inline constexpr uint32_t OsuGameDesktop_Ruleset = 1112;

    inline constexpr uint32_t RulesetInfo_OnlineID = 48;

    inline constexpr uint32_t ScoreInfo_ModsJson = 80;
}