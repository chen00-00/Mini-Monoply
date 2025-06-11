#include <iostream>
#include <iostream>
using namespace std;

class CPlayer {
public:
    virtual ~CPlayer() = default;
};

class CAmazon : public CPlayer {
public:
    CAmazon() { ++TotalNum; }
    CAmazon(const CAmazon &rhs) { ++TotalNum; }
    ~CAmazon() { --TotalNum; }

    static int TotalNumPlayers() { return TotalNum; }

private:
    static int TotalNum;
};

int CAmazon::TotalNum = 0;

class CPaladin : public CPlayer {
public:
    CPaladin() { ++TotalNum; }
    CPaladin(const CPaladin &rhs) { ++TotalNum; }
    ~CPaladin() { --TotalNum; }

    static int TotalNumPlayers() { return TotalNum; }

private:
    static int TotalNum;
};

int CPaladin::TotalNum = 0;

class CSorceress : public CPlayer {
public:
    CSorceress() { ++TotalNum; }
    CSorceress(const CSorceress &rhs) { ++TotalNum; }
    ~CSorceress() { --TotalNum; }

    static int TotalNumPlayers() { return TotalNum; }

private:
    static int TotalNum;
};

int CSorceress::TotalNum = 0;

void DisplayNumOfPlayer()
{
    cout << "We have " << CAmazon::TotalNumPlayers() << " amazons, "
                       << CPaladin::TotalNumPlayers() << " paladins, and "
                       << CSorceress::TotalNumPlayers() << " sorceress." << endl;
}

int main()
{
    DisplayNumOfPlayer();

    CPlayer *p[3] = {new CAmazon, new CPaladin, new CSorceress};

    DisplayNumOfPlayer();

    if (1)
    {
        CAmazon lisa;
        CPaladin tmp[2];

        DisplayNumOfPlayer();
    }

    DisplayNumOfPlayer();

    delete p[0];
    DisplayNumOfPlayer();
}