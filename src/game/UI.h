#ifndef PINESEED_GAME__UI_H
#define PINESEED_GAME__UI_H

#include <boost/enable_shared_from_this.hpp>

#include "kriti/render/RenderableContainer.h"

#include "kriti/gui/ScrollArea.h"
#include "kriti/gui/Panel.h"
#include "kriti/gui/Label.h"

#include "kriti/MessageSystem.h"

namespace Pineseed {
namespace Game {

class UI : public boost::enable_shared_from_this<UI> {
private:
    static UI *s_singleton;
public:
    static UI *instance() {
        if(!s_singleton) {
            Message3(Game, Fatal, "No UI instance created!");
        }
        return s_singleton;
    }
    static void destroy() {
        s_singleton = nullptr;
    }
private:
    boost::shared_ptr<Kriti::Render::RenderableContainer> m_container;

    boost::shared_ptr<Kriti::GUI::ScrollArea> m_scroll;
    boost::shared_ptr<Kriti::GUI::Panel> m_labelPanel;
    std::vector<boost::shared_ptr<Kriti::GUI::Label>> m_labels;
    boost::shared_ptr<Kriti::GUI::Panel> m_optionPanel;
    std::vector<boost::shared_ptr<Kriti::GUI::Label>> m_optionLabels;
public:
    UI();

    boost::shared_ptr<Kriti::Render::RenderableContainer> renderables() const
        { return m_container; }

    void update(boost::shared_ptr<Kriti::GUI::OutlineRegistry> registry);

    void addJournalText(std::string text);

    void addOption(std::string text);
    void clearOptions();
};

}  // namespace Game
}  // namespace Pineseed

#endif
