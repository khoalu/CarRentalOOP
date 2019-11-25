#ifndef IJSONHANDLE_H
#define IJSONHANDLE_H

#include <fstream>
#include <string>
#include <vector>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/prettywriter.h"

using namespace rapidjson;

/**
    interface(?) IJsonHandle: everything about JSON/DOM  read / write here
*/

class IJsonHandle
{
    public:
        //virtual Document getDomObject() = 0;
        virtual Document* getDomObject(std::vector<Document*>&) = 0;
        void printJsonToFile(std::string filePath)
        {
            std::vector<Document*> arr;
            Document *dom = this->getDomObject(arr);

            std::ofstream ofs(filePath);
            OStreamWrapper osw(ofs);
            PrettyWriter<OStreamWrapper> writer(osw);
            dom->Accept(writer);
            ofs.close();

            // memory leaked handle
            for(auto &x: arr) delete x;
            arr.clear();
        }
};

#endif // IJSONHANDLE_H
