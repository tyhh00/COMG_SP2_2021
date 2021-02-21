#include "InteractionQueue.h"

InteractionQueue::InteractionQueue() {

}

InteractionQueue::~InteractionQueue()
{
}

std::vector<Interaction*>& InteractionQueue::getQueue()
{
	return queue;
}

void InteractionQueue::pushInteraction(Interaction* interaction) {
	queue.push_back(interaction);
}

void InteractionQueue::pushInteraction(std::vector<Command*> preInteractionCMD, std::string interactionText, std::vector<Command*> postInteractionCMD) {
	Interaction* temp;
	temp = new Interaction();
	temp->preInteractionCMD = preInteractionCMD;
	temp->postInteractionCMD = postInteractionCMD;
	temp->interactionText = interactionText;
	queue.push_back(temp);
}

void InteractionQueue::popInteraction() {
	delete this->queue[0];
	this->queue.erase(std::remove(queue.begin(), queue.end(), nullptr), queue.end());
}