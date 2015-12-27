#include <boost/make_shared.hpp>

#include "UI.h"

#include "kriti/gui/PackedLayout.h"
#include "kriti/gui/Font.h"

#include "kriti/ResourceRegistry.h"
#include "kriti/MessageSystem.h"

using namespace Kriti;

namespace Pineseed {
namespace Game {

boost::shared_ptr<UI> UI::s_singleton;

UI::UI() {
    m_container = boost::make_shared<Render::RenderableContainer>();

    m_scrollPanel = boost::make_shared<GUI::Panel>(Math::Vector(.1,.1),
        Math::Vector(1,0),
        boost::make_shared<GUI::PackedLayout>(Math::Vector(1,0)));
    m_scroll = boost::make_shared<GUI::ScrollArea>(Math::Vector(),
        Math::Vector(), m_scrollPanel, Math::Vector(1,1));

    m_labelPanel = boost::make_shared<GUI::Panel>(Math::Vector(.1,.1),
        Math::Vector(1,0),
        boost::make_shared<GUI::PackedLayout>(Math::Vector(1,0)));
    m_optionPanel = boost::make_shared<GUI::Panel>(Math::Vector(.1,.1),
        Math::Vector(1,0),
        boost::make_shared<GUI::PackedLayout>(Math::Vector(1,0)));

    m_scrollPanel->layout()->addItem(m_labelPanel);
    m_scrollPanel->layout()->addItem(m_optionPanel);
}

void UI::update(boost::shared_ptr<Kriti::GUI::OutlineRegistry> registry) {
    Math::Vector pos(1.2, 0.0);
    Math::Vector size(16/9.0 - 1.2, 1.0);
    m_scroll->scrollSize() = m_scrollPanel->minSize();
    m_scroll->scrollSize().setX(size.x());
    m_scroll->update(registry, pos, size, Math::Vector(1,1));

    m_scroll->fill(m_container);
    m_labelPanel->fill(m_container);
}

void UI::addJournalText(std::string text) {
    auto font = ResourceRegistry::get<GUI::Font>("Ubuntu-B.ttf");
    auto label = boost::make_shared<GUI::Label>(Math::Vector(0,.1),
        Math::Vector(1,0), font->getInstance(12), text);

    m_labels.push_back(label);
    m_labelPanel->layout()->addItem(label);
}

void UI::addOption(std::string text) {
    auto font = ResourceRegistry::get<GUI::Font>("Ubuntu-R.ttf");
    auto label = boost::make_shared<GUI::Label>(Math::Vector(0,.1),
        Math::Vector(1,0), font->getInstance(12),
        Kriti::StreamAsString() << "#" << m_optionLabels.size()+1 << ": " << text,
        Math::Vector(1.0, 1.0, 0.0));

    m_optionLabels.push_back(label);
    m_optionPanel->layout()->addItem(label);
}

void UI::clearOptions() {
    m_optionPanel->layout()->flush(m_container);
    m_optionPanel->layout()->clearItems();
}

}  // namespace Game
}  // namespace Pineseed
