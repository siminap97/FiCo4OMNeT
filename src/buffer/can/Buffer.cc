#include "Buffer.h"

void Buffer::handleMessage(cMessage *msg) {
    if (msg->arrivedOn("in")) {
        DataFrame *frame = check_and_cast<DataFrame *>(msg);
        putFrame(frame);
    }
}

DataFrame* Buffer::getFrame(int id) {
    for (std::list<DataFrame*>::iterator it = frames.begin();
            it != frames.end(); ++it) {
        DataFrame *tmp = *it;
        if ((tmp->getId() == id)) {
            return tmp;
        }
    }
    return NULL;
}

void Buffer::putFrame(DataFrame* frame) {
    frames.push_back(frame);
    for (std::list<cGate*>::iterator it = destinationGates.begin();
            it != destinationGates.end(); ++it) {
        cGate *tmpGate = *it;
        cMessage *msg = new cMessage("Message in Buffer");
        sendDirect(msg, tmpGate->getOwnerModule()->gate("bufferIn"));
    }
}

void Buffer::deleteFrame(int id) {
    DataFrame *tmp = getFrame(id);
    frames.remove(tmp);
}

void Buffer::deliverFrame(int id) {
    sendToDestinationGates(getFrame(id));
}

void Buffer::deliverPrioFrame() {
    int prioId = INT_MAX;
    DataFrame *prioFrame;
    for (std::list<DataFrame*>::iterator it = frames.begin();
            it != frames.end(); ++it) {
        DataFrame *tmp = *it;
        if ((tmp->getId() < prioId)) {
            prioFrame = tmp;
            prioId = tmp->getId();
        }
    }
    sendToDestinationGates(prioFrame);
}

void Buffer::deliverNextFrame() {
    sendToDestinationGates(frames.front());
}

void Buffer::sendToDestinationGates(DataFrame *df) {
    for (std::list<cGate*>::iterator it = destinationGates.begin();
            it != destinationGates.end(); ++it) {
        cGate *tmpGate = *it;
        sendDirect(df->dup(), tmpGate);
    }
    delete df;
}
