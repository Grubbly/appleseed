
//
// This source file is part of appleseed.
// Visit http://appleseedhq.net/ for additional information and resources.
//
// This software is released under the MIT license.
//
// Copyright (c) 2010-2013 Francois Beaune, Jupiter Jazz Limited
// Copyright (c) 2014-2017 Francois Beaune, The appleseedhq Organization
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// Interface header.
#include "attributeeditor.h"

// appleseed.studio headers.
#include "mainwindow/project/ientityvalueprovider.h"
#include "utility/miscellaneous.h"

// appleseed.renderer headers.
#include "renderer/utility/paramarray.h"

// Qt headers.
#include <QLayout>

// Standard headers.
#include <utility>

using namespace foundation;
using namespace renderer;
using namespace std;

namespace appleseed {
namespace studio {

AttributeEditor::AttributeEditor(
    QWidget*    parent,
    Project&    project,
    ParamArray& settings)
  : m_parent(parent)
  , m_project(project)
  , m_settings(settings)
{
}

void AttributeEditor::clear()
{
    if (m_parent->layout())
    {
        clear_layout(m_parent->layout());
        delete m_parent->layout();
        m_entity_editor.reset();
        m_value_provider = nullptr;
    }
}

void AttributeEditor::edit(
    unique_ptr<EntityEditor::IFormFactory>      form_factory,
    unique_ptr<EntityEditor::IEntityBrowser>    entity_browser,
    unique_ptr<CustomEntityUI>                  custom_ui,
    const Dictionary&                           values,
    QObject*                                    receiver,
    const char*                                 slot_apply)
{
    IEntityValueProvider* value_provider = dynamic_cast<IEntityValueProvider*>(receiver);
    if (value_provider)
        m_value_provider = value_provider;

    m_entity_editor.reset(
        new EntityEditor(
            m_parent,
            m_project,
            m_settings,
            move(form_factory),
            move(entity_browser),
            move(custom_ui),
            values));

    QObject::connect(
        m_entity_editor.get(), SIGNAL(signal_applied(foundation::Dictionary)),
        receiver, slot_apply);
}

void AttributeEditor::refresh() const
{
    if (m_entity_editor.get() && m_value_provider)
        m_entity_editor.get()->rebuild_form(m_value_provider->get_values());
}

}   // namespace studio
}   // namespace appleseed
