#include <boost/make_shared.hpp>

#include "UI.h"

#include "kriti/gui/PackedLayout.h"
#include "kriti/gui/Font.h"

#include "kriti/ResourceRegistry.h"
#include "kriti/MessageSystem.h"

using namespace Kriti;

namespace Pineseed {
namespace Game {

UI *UI::s_singleton;

UI::UI() {
    s_singleton = this;
    m_container = boost::make_shared<Render::RenderableContainer>();

    m_labelPanel = boost::make_shared<GUI::Panel>(Math::Vector(.1,.1),
        Math::Vector(1,0),
        boost::make_shared<GUI::PackedLayout>(Math::Vector(1,0)));
    m_scroll = boost::make_shared<GUI::ScrollArea>(Math::Vector(),
        Math::Vector(), m_labelPanel, Math::Vector(1,1));

    m_optionPanel = boost::make_shared<GUI::Panel>(Math::Vector(.1,.1),
        Math::Vector(1,0),
        boost::make_shared<GUI::PackedLayout>(Math::Vector(1,0)));
}

void UI::update(boost::shared_ptr<Kriti::GUI::OutlineRegistry> registry) {
    m_scroll->fill(m_container);
    m_labelPanel->fill(m_container);

    Math::Vector pos(0.8, 0.0);
    Math::Vector size(0.4, 1.0);
    m_scroll->scrollSize() = m_labelPanel->minSize();
    m_scroll->scrollSize().setX(size.x());
    m_scroll->update(registry, pos, size, Math::Vector(1,1));
}

void UI::addJournalText(std::string text) {
    auto font = ResourceRegistry::get<GUI::Font>("Ubuntu-B.ttf");
    auto label = boost::make_shared<GUI::Label>(Math::Vector(0,.1), Math::Vector(1,0), font->getInstance(12), text);

    m_labels.push_back(label);
    m_labelPanel->layout()->addItem(label);
}

void UI::addOption(std::string text) {
    auto font = ResourceRegistry::get<GUI::Font>("Ubuntu-B.ttf");
    auto label = boost::make_shared<GUI::Label>(Math::Vector(0,.1), Math::Vector(1,0), font->getInstance(12), text, Math::Vector(1.0, 1.0, 0.0));

    m_optionLabels.push_back(label);
    m_optionPanel->layout()->addItem(label);
}

void UI::clearOptions() {
    m_optionPanel->layout()->flush(m_container);
    m_optionPanel->layout()->clearItems();
}

}  // namespace Game
}  // namespace Pineseed
