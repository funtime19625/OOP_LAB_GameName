//
// Created by fungp on 2024/5/24.
//
#ifndef SELECTEAMMATE_HPP
#define SELECTEAMMATE_HPP
#include "iostream"
#include "memory"
#include "vector"
#include <functional>
#include "Member.hpp"
#include "Datas.hpp"



class Team{
public:

    std::vector<std::shared_ptr<Member>>  GetTeam();
    int Size();
    void LoadMemberFile(MemberSettingData target);
protected:
    std::vector<std::shared_ptr<Member>> m_MemberList;
    MemberSettingData m_Data;
};

class NorthTeam : public Team{
public:
    NorthTeam();
};
#endif // SELECTTEAMMATE_HPP
