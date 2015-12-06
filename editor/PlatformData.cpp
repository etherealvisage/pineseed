#include "PlatformData.h"

PlatformData::PlatformData() {
    
}

int PlatformData::getAvailableID() {
    int ret = -1;
    do {
        ret = qrand();
    } while(m_usedIDs.contains(ret));

    m_usedIDs.insert(ret);
    return ret;
}

void PlatformData::serialize(QXmlStreamWriter &xml) {
    
}

void PlatformData::deserialize(QDomDocument &doc) {

}
