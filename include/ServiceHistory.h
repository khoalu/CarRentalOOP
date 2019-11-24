#ifndef SERVICE_HISTORY_H
#define SERVICE_HISTORY_H

#include <vector>
#include "Datetime.h"

#include "IJSONHandle.h"

using namespace std;

enum MaintenanceJobType
{
    OIL_CHANGE,
    FLUID_CHANGE,
    TIRE_ADJUSTMENT,
    TIRE_REPLACEMENT,
    TRANSMISSION_OVERHAUL,

    NUM_OF_MAINTENANCE_JOB_TYPE
};

class MaintenanceJob: public IJsonHandle{
protected:

    MaintenanceJobType name;
    TDateTime beginDay;
    TDateTime endDay;

    int cost;
    bool beenPaid;

    int mileage;
    string location;

    static string jobName[(int)MaintenanceJobType::NUM_OF_MAINTENANCE_JOB_TYPE];
    static void initJobName()
    {
        static bool flag = false;
        if (flag) return;
        flag = true;
        jobName[MaintenanceJobType::OIL_CHANGE] = "oil_change";
        jobName[MaintenanceJobType::FLUID_CHANGE] = "fluid_change";
        jobName[MaintenanceJobType::TIRE_ADJUSTMENT] = "tire_adjustment";
        jobName[MaintenanceJobType::TIRE_REPLACEMENT] = "tire_replacement";
        jobName[MaintenanceJobType::TRANSMISSION_OVERHAUL] = "transmission_overhaul";
    }
    MaintenanceJobType str2job(string str)
    {
        for(int i = 0; i < MaintenanceJobType::NUM_OF_MAINTENANCE_JOB_TYPE; ++i)
            if (jobName[i] == str)
                return MaintenanceJobType(i);
    }

    string job2str(MaintenanceJobType type) const {return jobName[type];}

public:
    MaintenanceJob() {initJobName();}
    MaintenanceJob(MaintenanceJobType name, int mileage, TDateTime beginDay, int cost, string location)
        : name(name), beginDay(beginDay), endDay(TDateTime(TDate(0,0,0))),
            cost(cost), beenPaid(false), location(location)
            {
                initJobName();
            }
    MaintenanceJob(MaintenanceJobType name, int mileage, TDateTime beginDay, TDateTime endDay)
        : name(name), beginDay(beginDay), endDay(endDay), mileage(mileage)
        {
            initJobName();
        }
    MaintenanceJob(MaintenanceJobType name, int mileage, TDateTime beginDay)
        : name(name), beginDay(beginDay), mileage(mileage)
        {
            initJobName();
        }
    MaintenanceJob(MaintenanceJobType name, int mileage, int cost)
        : name(name), beginDay(TDate(1111,1,1)), cost(cost), mileage(mileage)
        {
            initJobName();
        }

    virtual ~MaintenanceJob() {}

    string getName() const {return job2str(this->name);}

    void setBeginDay(TDateTime beginDay){this->beginDay = beginDay;}
    TDateTime getBeginDay() const {return beginDay;}

    void setEndDay(TDateTime endDay){this->endDay = endDay;}
    TDateTime getEndDay() const {return endDay;}

    int getCost() const {return cost;}

    void setBeenPaid(bool bennPaid) {this->beenPaid = beenPaid;}
    bool getBeenPaid() const {return beenPaid;}

    int getMileage() const {return mileage;}

    int operator-(const MaintenanceJob& that)
    {
        return this->mileage - that.mileage;
    }

    bool operator==(const MaintenanceJob& that)
    {
        return that.mileage == this->mileage;
    }
    bool operator<(const MaintenanceJob& that)
    {
        return this->mileage < that.mileage;
    }

    bool operator!=(const MaintenanceJob& that)
    {
        return !(*this == that);
    }
    bool operator>(const MaintenanceJob& that)
    {
        return !(*this < that || *this == that);
    }

    friend std::ostream& operator<<(std::ostream &os, const MaintenanceJob &job)
    {
        os << "name = " << job.getName() << "\n";
        os << "fee = " << job.getCost() << "\n";
        os << "start day = " << job.getBeginDay() << "\n";
        os << "finish day = " << job.getEndDay() << "\n";
        return os;
    }

    Document* getDomObject(vector<Document*> &arr)
    {
        Document *d = new Document();
        d->SetObject();
        auto &a = d->GetAllocator();
        d->AddMember("name", StringRef(jobName[name].c_str()),a);
        string DomDay = beginDay.toJson(); Value s;
        s.SetString(DomDay.data(), DomDay.size(), a);
        d->AddMember("beginDay", s, a);
        DomDay = endDay.toJson();
        s.SetString(DomDay.data(), DomDay.size(), a);
        d->AddMember("endDay", s, a);

        d->AddMember("cost", cost, a);
        d->AddMember("beenPaid", beenPaid, a);
        d->AddMember("mileage",mileage, a);
        d->AddMember("location", StringRef(location.c_str()), a);
        arr.push_back(d);
        return d;
    }
};

string MaintenanceJob::jobName[(int)MaintenanceJobType::NUM_OF_MAINTENANCE_JOB_TYPE];

class ServiceHistory: public IJsonHandle {
protected:
    vector<MaintenanceJob*> data;
    int totalFee, paidFee, debtFee;
public:
    ServiceHistory(): totalFee(0), paidFee(0), debtFee(0) {}
    virtual ~ServiceHistory() {
        for(auto &x: data) delete x;
        data.clear();
    }

    MaintenanceJob*& operator[](int i) {return data[i];}

    void addService(MaintenanceJob* job)
    {
        data.push_back(job);
        totalFee += job->getCost();
        debtFee += job->getCost();
    }
    void payService(int jobId)
    {
        if (data[jobId]->getBeenPaid())
        {
            cout << "this has been paid !";
        } else
        {
            paidFee += data[jobId]->getCost();
            debtFee -= data[jobId]->getCost();
        }
    }
    void printHistory()
    {
        cout << "History of services: \n";
        for (auto &job: data)
        {
            cout << "---------------------\n";
            cout << (*job);
        }
        cout << "---------------------\n";
        cout << "total fee = " << totalFee << "\n";
        cout << "paid fee = " << paidFee << "\n";
        cout << "debt free = " << debtFee << "\n";
        cout << "---------------------\n\n";
    }

    Document* getDomObject(vector<Document*> &arr)
    {
        Document *d = new Document();
        d->SetObject();
        auto &a = d->GetAllocator();
        Value jobList(kArrayType);
        for(auto &job: data)
        {
            jobList.PushBack(*(job->getDomObject(arr)),a);
        }
        d->AddMember("data", jobList, a);
        arr.push_back(d);
        return d;
    }

};

#endif // SERVICE_HISTORY_H
