#ifndef PINESEED_GAME__UI_H
#define PINESEED_GAME__UI_H

#include "kriti/render/RenderableContainer.h"

#include "kriti/gui/ScrollArea.h"
#include "kriti/gui/Panel.h"
#include "kriti/gui/Label.h"

namespace Pineseed {
namespace Game {

class UI {
private:
    boost::shared_ptr<Kriti::Render::RenderableContainer> m_container;

    boost::shared_ptr<Kriti::GUI::ScrollArea> m_scroll;
    boost::shared_ptr<Kriti::GUI::Panel> m_labelPanel;
    std::vector<boost::shared_ptr<Kriti::GUI::Label>> m_labels;
public:
    UI();

    boost::shared_ptr<Kriti::Render::RenderableContainer> renderables() const
        { return m_container; }

    void update(boost::shared_ptr<Kriti::GUI::OutlineRegistry> registry);

    void addText(std::string text);
};

}  // namespace Game
}  // namespace Pineseed

#endif
