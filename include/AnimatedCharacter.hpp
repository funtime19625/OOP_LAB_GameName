#ifndef ANIMATED_CHARACTER_HPP
#define ANIMATED_CHARACTER_HPP

#include <vector>
#include <string>
#include <memory>

#include "Util/Animation.hpp"
#include "Util/GameObject.hpp"


class AnimatedCharacter : public Util::GameObject {

public:
    void SetImg(const std::vector<std::string>& AnimationPaths);

    explicit AnimatedCharacter(const std::vector<std::string>& AnimationPaths);
    [[nodiscard]] bool IsLooping() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetLooping();
    }

    [[nodiscard]] bool IsPlaying() const {
        return std::dynamic_pointer_cast<Util::Animation>(m_Drawable)->GetState() == Util::Animation::State::PLAY;
    }

    void SetLooping(bool looping) {
        auto temp = std::dynamic_pointer_cast<Util::Animation>(m_Drawable);
        temp->SetLooping(looping);
    }
    void SetPlaying(bool playing) {
        if(playing){
            m_Animation->Play();
        }else{
            m_Animation->Pause();
        }
    }
    [[nodiscard]] bool IfAnimationEnds() const;

    void SetAnimation(std::shared_ptr<Util::Animation> target){
        m_Animation = target;
        SetDrawable(m_Animation);
    }

protected:
    std::shared_ptr<Util::Animation> m_Animation;
};

#endif //ANIMATED_CHARACTER_HPP
