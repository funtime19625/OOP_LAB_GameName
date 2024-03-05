#include "Enchant.hpp"
#include "Util/input.hpp"
#include "Util/SFX.hpp"
#include <cmath>
void Enchant::Start() {
    m_row = 6 , m_column = 5;
    m_Array.resize(m_row);
    for (int i = 0; i < m_row; ++i) {
        m_Array[i].resize(m_column);
        for (int j = 0; j < m_column; ++j) {
            m_Array[i][j] = std::make_shared<Stone>(); // 使用 make_shared 創建共享指標
            m_Array[i][j]->Start(i,j);
        }
    }
    m_state = state::Keeping;
    this->Draw();
}

void Enchant::Update() {

    for (int i = 0; i < m_row; ++i) {
        for (int j = 0; j < m_column; ++j) {
            if(m_Array[i][j] != nullptr){
                m_Array[i][j]->Update();
            }
        }
    }
    switch (m_state) {
    case state::Keeping:
        KeepingStateUpdate();
        break;
    case state::Dragging:
        DraggingStateUpdate();
        break;
    case state::Moving:
        MovingStateUpdate();
        break;
    case state::Checking:
        CheckingStateUpdate();
        break;
    case state::Falling:
        KeepingStateUpdate();
        break;
    }
}

void Enchant::Change(glm::vec2 pos1,glm::vec2 pos2){
    static auto SFX = Util::SFX("../assets/audio/Click.wav");
    SFX.Play();
    m_Array[pos1.x][pos1.y]->Change(m_Array[pos2.x][pos2.y]);
    std::swap(m_Array[int(pos1.x)][int(pos1.y)], m_Array[int(pos2.x)][int(pos2.y)]);
}

bool Enchant::CheckMatch() {
    for(int i=0;i<m_row;++i){
        for(int j=0;j<m_column;j++){
            //LOG_DEBUG("Checking [{},{}]",i+1,j+1);
            if(m_Array[i][j] != nullptr){
                std::vector<std::shared_ptr<Stone>> breakList;
                breakList.push_back(m_Array[i][j]);
                while (j + breakList.size() < m_column){
                    if(m_Array[i][j+breakList.size()] != nullptr){
                        if(m_Array[i][j]->GetType() == m_Array[i][j+breakList.size()]->GetType()){
                            breakList.push_back(m_Array[i][j+breakList.size()]);
                        }else{
                            break;
                        }
                    }else{
                        break;
                    }
                }
                if(breakList.size() >= 3){
                    for(int k=0;k<breakList.size();k++){
                        LOG_DEBUG("erase [{},{}]",breakList[k]->GetRow()+1,breakList[k]->GetColumn()+1);
                        m_Array[i][j+k].reset();
                    }
                    return true;
                }else{
                    //LOG_DEBUG("breakList has {} blocks",breakList.size());
                }
            }
        }
    }
    for(int i=0;i<m_row;++i) {
        for (int j = 0; j < m_column; j++) {
            // LOG_DEBUG("Checking [{},{}]",i+1,j+1);
            if (m_Array[i][j] != nullptr) {
                std::vector<std::shared_ptr<Stone>> breakList;
                breakList.push_back(m_Array[i][j]);
                while (i + breakList.size() < m_row) {
                    if (m_Array[i + breakList.size()][j] != nullptr) {
                        if (m_Array[i][j]->GetType() ==
                            m_Array[i + breakList.size()][j]->GetType()) {
                            breakList.push_back(
                                m_Array[i + breakList.size()][j]);
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                }
                if (breakList.size() >= 3) {
                    for (int k = 0; k < breakList.size(); k++) {
                        LOG_DEBUG("erase [{},{}]", breakList[k]->GetRow() + 1,
                                  breakList[k]->GetColumn() + 1);
                        m_Array[i+k][j].reset();
                    }
                    return true;
                } else {
                    // LOG_DEBUG("breakList has {} blocks",breakList.size());
                }
            }
        }
    }
    return false;
}

void Enchant::DoFall() {
    for(int i=0;i<m_row;++i) {
        for(int loop=0;loop<4;loop++) {
            for (int j = 1; j < m_column; j++) {
                if (m_Array[i][j] != nullptr) {
                    if (m_Array[i][j - 1] == nullptr) {
                        m_Array[i][j - 1] = m_Array[i][j];
                        m_Array[i][j - 1]->SetPos(i, j - 1);
                        m_Array[i][j-1]->SetFalling();
                        m_Array[i][j].reset();
                    }
                }
            }
        }
    }
    for(int i=0;i<m_row;++i) {
        for (int j = 0; j < m_column; j++) {

        }
    }

}

void Enchant::ShowEnchant() {
    for(int i=0;i<m_column;++i) {
        std::string lists = "";
        for (int j = 0; j < m_row; j++) {
            if(m_Array[j][4-i] != nullptr){
                lists += "V";
            }else{
                lists += "X";
            }
        }
        LOG_DEBUG("{}",lists);
    }
    LOG_DEBUG("-----------");
}

void Enchant::GenerateFall() {
    for(int i=0;i<m_row;i++){
        for(int j=0;j<m_column;j++){
            if(m_Array[i][j] == nullptr){
                m_Array[i][j] = std::make_shared<Stone>(); // 使用 make_shared 創建共享指標
                m_Array[i][j]->Generate(i,j);
            }
        }
    }

}

bool Enchant::CheckFall() {
    for(int i=0;i<m_row;i++) {
        for (int j = 0; j < m_column; j++) {
            if(m_Array[i][j]->GetState() != Stone::state::Keeping){
                return false;
            }
        }
    }
    return true;
}

bool Enchant::CheckFull(){
    for(int i=0;i<m_row;i++){
        for(int j=0;j<m_column;j++){
            if(m_Array[i][j] == nullptr){
                return false;
            }
        }
    }
    return true;
}

/*method*/
void Enchant::KeepingStateUpdate() {
    const int row = 6, column = 5;
    if (Util::Input::IsKeyDown(Util::Keycode::E)) {
        auto cursorPos = Util::Input::GetCursorPosition();
        int i = std::clamp(static_cast<int>(std::floor((cursorPos.x + 225) / 75)), 0, 5);
        int j = std::clamp(static_cast<int>(std::floor((cursorPos.y + 350) / 78)), 0, 4);
        m_Array[i][j]->SetDragging(true);
        m_StartPos = glm::vec2(i, j);
        m_NowPos = m_StartPos;
        LOG_DEBUG("you got the {} row, {} column Stone", i + 1, j + 1);
        m_state = state::Dragging;
    }
}

void Enchant::DraggingStateUpdate() {
    if (Util::Input::IsMouseMoving()) {
        auto cursorPos = Util::Input::GetCursorPosition();
        int i = std::clamp(static_cast<int>(std::floor((cursorPos.x + 225) / 75)), 0, 5);
        int j = std::clamp(static_cast<int>(std::floor((cursorPos.y + 350) / 78)), 0, 4);
        if (!(m_StartPos == glm::vec2(i, j))) {
            LOG_DEBUG("Let's Moving to [{}, {}] from [{}, {}]", i + 1, j + 1, static_cast<int>(m_NowPos.x) + 1, static_cast<int>(m_NowPos.y) + 1);
            Change(m_StartPos, glm::vec2(i, j));
            m_NowPos = glm::vec2(i, j);
            m_state = state::Moving;
            return;
        }
    }
    if (Util::Input::IsKeyUp(Util::Keycode::E)) {
        m_Array[static_cast<int>(m_StartPos.x)][static_cast<int>(m_StartPos.y)]->SetDragging(false);
        m_state = state::Keeping;
        LOG_DEBUG("you put the {} row, {} column Stone", static_cast<int>(m_StartPos.x) + 1, static_cast<int>(m_StartPos.y) + 1);
    }
}

void Enchant::MovingStateUpdate() {
    if (Util::Input::IsMouseMoving()) {
        auto cursorPos = Util::Input::GetCursorPosition();
        int i = std::clamp(static_cast<int>(std::floor((cursorPos.x + 225) / 75)), 0, 5);
        int j = std::clamp(static_cast<int>(std::floor((cursorPos.y + 350) / 78)), 0, 4);
        if (!(m_NowPos == glm::vec2(i, j))) {
            Change(m_NowPos, glm::vec2(i, j));
            m_NowPos = glm::vec2(i, j);
            LOG_DEBUG("You are now at [{}, {}]", static_cast<int>(m_NowPos.x) + 1, static_cast<int>(m_NowPos.y) + 1);
            return;
        }
    }
    if (Util::Input::IsKeyUp(Util::Keycode::E)) {
        m_Array[static_cast<int>(m_StartPos.x)][static_cast<int>(m_StartPos.y)]->SetDragging(false);
        m_EndPos = m_NowPos;
        m_state = state::Checking;
    }
}
void Enchant::CheckingStateUpdate() {
    const int row = 6, column = 5;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            m_Array[i][j]->SetDragging(false);
        }
    }
    LOG_DEBUG("you are Dragging Start in the {} row, {} column", static_cast<int>(m_StartPos.x), static_cast<int>(m_StartPos.y));
    LOG_DEBUG("you are Dragging End in the {} row, {} column", static_cast<int>(m_EndPos.x), static_cast<int>(m_EndPos.y));
    m_state = state::Falling;
}
void Enchant::FallingStateUpdate() {
    m_state = state::Keeping;
}

