#ifndef ENCHANT_HPP
#define ENCHANT_HPP

#include "Stone.hpp"
#include "Util/GameObject.hpp"
#include "BattleSystem.hpp"

class Enchant : public Util::GameObject,std::enable_shared_from_this<Enchant>{
public:
    enum class state{
        Falling,
        Checking,
        Explosing,
        Keeping,
        Dragging,
        Moving,
        Unlimited
    };
    int CountEmpty();
    void SetFallingValue(std::vector<int> value);
    void Update();
    void Start();
    void CheckMatch();
    void DoFall();
    bool CheckFall();
    void MustFall(Type::Element_type type,int howmuch,bool powerup);
    void GenerateFall(bool powerup);
    void SetDraggingTime(int time);
    void SetState(state target);
    float StoneBreak(Type::Element_type LValue);
    void StoneTurn(Type::Element_type LValue,Type::Element_type RValue,int howmany,bool powerup);
    void SetSystem(std::shared_ptr<BattleSystem> target);
    std::vector<int> GetTypeGeneration();
    std::shared_ptr<Enchant> getEnchant();
    void SetTypeGeneration(std::vector<int> lists);
    Enchant() : Util::GameObject(std::make_unique<Util::Image>("../assets/sprites/Giraffe.png"), 1){};


private:
    std::weak_ptr<Enchant> m_weakptr;
    std::shared_ptr<BattleSystem> m_battleSystem;
    std::shared_ptr<GiraffeText> m_DragTimeText = std::make_shared<GiraffeText>("../assets/fonts/Inter.ttf",15);
    std::vector<std::vector<std::shared_ptr<Stone>>> m_explosionBar;
    std::vector<std::vector<std::shared_ptr<Stone>>> m_Array;
    std::vector<std::vector<std::shared_ptr<Stone>>> organizePairs();
    std::vector<std::shared_ptr<Stone>> mergePairs(const std::vector<std::shared_ptr<Stone>>& pair1, const std::vector<std::shared_ptr<Stone>>& pair2);
    bool checkOverlap(const std::vector<std::shared_ptr<Stone>>& pair1, const std::vector<std::shared_ptr<Stone>>& pair2);
    state m_state=state::Falling;
    int m_row;
    int m_column;
    float m_DraggingTime;
    bool m_firstBreak;
    bool m_unlimited;
    glm::vec2 m_StartPos;
    glm::vec2 m_NowPos;
    glm::vec2 m_EndPos;
    std::vector<int> m_TypeGeneration;
    std::vector<int> m_mustFallbyNormal;
    std::vector<int> m_mustFallbyPowerup;

    /*method*/
    std::weak_ptr<Enchant> getWeakPtr();
    void KeepingStateUpdate();
    void DraggingStateUpdate();
    void MovingStateUpdate();
    void CheckingStateUpdate();
    void FallingStateUpdate();
    void ExplosingStateUpdate();
    void UnlimitedStateUpdate();
    bool CheckFull();
    void ShowEnchant();
    void Change(glm::vec2 pos1,glm::vec2 pos2);
    void ShowExplosionBar();
    void ExplosionBarClean();
};



#endif