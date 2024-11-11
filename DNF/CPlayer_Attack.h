#pragma once
#include "CState.h"
#include "CKeyMgr.h"

enum class PlayerSkill
{
    Geardrive,
    Mistheavyrain,
    Setupfire,
    Hazestorm,
    Nebula,
    Scrambleanchor,
    Acrobaticraid,
    Jetstream,
    Mistcroud,
    Setuprage,
    Dizzle,
    Tripleaxel,
    Polaris,
    Sagittarius,

    Basic0,
    Basic1,
    Basic2,
    Exciting,
    Setupattack,

    END
};

vector<Keyboard> AttackKey =
{
    Keyboard::Q,
    Keyboard::W,
    Keyboard::E,
    Keyboard::R,
    Keyboard::T,
    Keyboard::Y,
    Keyboard::A,
    Keyboard::S,
    Keyboard::D,
    Keyboard::F,
    Keyboard::G,
    Keyboard::H,
    Keyboard::NUM_4,
    Keyboard::NUM_5,

    Keyboard::Z,
    Keyboard::LCTRL

};

enum class EffectLayer
{
    a,
    b,
    c,
    d,

    END
};

class CAlbumPlayer;
class CArrow;
class CDummy;

class CPlayer_Attack :
    public CState
{
    friend class CTimeMgr;
public:
    CPlayer_Attack(wstring _name);
    ~CPlayer_Attack();

private:
    float m_SkillDuration;
    float m_PlaylistTimer;
    float m_PlaylistEffTimer[(int)EffectLayer::END];
    float m_PlaylistScrTimer[(int)EffectLayer::END];
    float m_IdleTimer;
    float m_Cooltime[(int)PlayerSkill::END];
    PlayerSkill m_CurrentSkill;
    Keyboard m_TappedKey;
    vector<pair<CAlbumPlayer*, float>> m_Playlist;
    vector<pair<vector<CAlbumPlayer*>, float>> m_PlaylistAvt;
    vector<pair<CAlbumPlayer*, float>> m_PlaylistEff[(int)EffectLayer::END];
    vector<pair<CAlbumPlayer*, float>> m_PlaylistScr[(int)EffectLayer::END];
    int m_PlaylistIdx;
    int m_PlaylistEffIdx[(int)EffectLayer::END];
    int m_PlaylistScrIdx[(int)EffectLayer::END];
    CAlbumPlayer* m_IdleAnimation;
    vector<CAlbumPlayer*> m_IdleAnimationAvt;
    CDummy* m_Screen;

    bool m_Flag[5];
    Vec2D m_Coord;
    WCHAR m_SagittariusCode;


public:
    virtual void Enter();
    virtual void FinalTick();
    virtual void Exit();

    void Render();
    void AddPlaylist(CAlbumPlayer* _AP, float _dur = 0);
    void AddPlaylistAvt(vector<CAlbumPlayer*> _APVec, float _dur = 0);
    void AddPlaylistEff(CAlbumPlayer* _AP, EffectLayer _layer, float _dur = 0);
    void AddPlaylistScr(CAlbumPlayer* _AP, EffectLayer _layer, float _dur = 0);
    void ClearAllPlaylist();
    void ClearPlaylist();
    void ClearEffPlaylist(int i);
    void ClearScrPlaylist(int i);

    bool AttackCheck();

private:
    void GeardriveCallback(DWORD_PTR _arrow);
    void MHRArrowCyan(int _angle);
    void MHRArrowCyanCallback0(DWORD_PTR _arrow);
    void MHRArrowCyanCallback1(DWORD_PTR _arrow);

};

