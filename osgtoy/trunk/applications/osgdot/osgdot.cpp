/*

  osgdot -- Output the relations between scenegraph nodes and
            drawables in graphviz's "dot" tool format.

   Paul E.C. Melis (paul@floorball-flamingos.nl), November, 2007

This software is hereby placed in the public domain. Use in whichever
way you see fit.

HE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

This is a small tool that reads in a model file (in any format readable
by OSG) and outputs a text file (graph.dot) that describes the connections
between nodes and some other details. The text file is in a format that
can be read by the "dot" tool from the graphviz package. With dot you can
generate a picture representing the scenegraph. For example, to generate
a PNG file from the graph.dot file use the command:

    dot -Tpng -ograph.png graph.dot

* osgdot takes a single argument, the model file to read
* The graph will show nodes and drawable as boxes, labeled with their OSG
  class name. If the node/drawable's getName() method returned a non-empty
  string this will also be shown in the node (below the horizontal line).
* Nodes are shown with a thick border, drawables with a thin border
* Nodes or drawables that have a stateset assigned to them will be filled
  with a grey color.
* Arrows between nodes/drawables indicate a parent -> child relationship.
  Arrows from node to node are shown with a solid line, arrows from node
  to drawable with a dashed line.
* For drawables of the class Geometry the number of primitive sets held
  by that object is shown below the horizontal line.

The graphviz package can be found at http://www.graphviz.org/.
Or check your Linux distro for the package. It also seems to be available
for Windows and MacOS.

Compile (on Linux and using gcc) with:

    g++ -W -Wall -o osgdot osgdot.cpp -I <osg-dir>/include -L <osg-dir>/lib -losg -losgDB

Version used during development: OSG 2.1.1, graphviz 2.12

*/

// mew@mew.cx 2011-01-10 : replace unsigned int and many casts with osg::Object*


#include <cstdio>
#include <string>
#include <map>

#include <osg/Node>
#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/Group>
#include <osg/Geode>
#include <osgDB/ReadFile>

class GraphNode
{
public:

    GraphNode(osg::Node *node)
    {
        char s[1024];

        _node = node;
        mem_location =  node;

        osg_class = node->className();
        name = node->getName();

        has_stateset = node->getStateSet() != NULL;

        sprintf(s, "%s_%p", osg_class.c_str(), mem_location);
                dot_id = std::string(s);

        type = "node";
                extra_label = "";
    }

    GraphNode(osg::Drawable *drawable)
    {
        char s[1024];

        _drawable = drawable;
        mem_location =  drawable;

        osg_class = drawable->className();
        name = drawable->getName();

        has_stateset = drawable->getStateSet() != NULL;

        sprintf(s, "%s_%p", osg_class.c_str(), mem_location);
                dot_id = std::string(s);

        type = "drawable";

        extra_label = "";
        if (dynamic_cast<osg::Geometry*>(drawable))
        {
            // include count of primitive sets
            sprintf(s, "%d psets", dynamic_cast<osg::Geometry*>(drawable)->getNumPrimitiveSets());
            extra_label = std::string(s);
        }
    }

    void
    add_target( osg::Object* t )
        {
        target_mem_locations.push_back(t);
    }

protected:
    osg::ref_ptr<osg::Node>         _node;
    osg::ref_ptr<osg::Drawable>     _drawable;

public:
    osg::Object*                mem_location;

    std::string                 osg_class;
    std::string                 name;
    std::string                 type;

    bool                        has_stateset;

    std::string                 dot_id;
    std::string                 extra_label;

    // stores nodes (mem locations) that this node references
    // (i.e. other nodes)
    std::vector<osg::Object*>   target_mem_locations;
};

typedef std::map<osg::Object*, GraphNode*>    GraphNodeMap;

void
traverse(GraphNodeMap& graphnodemap, osg::Node *node)
{
    GraphNode       *gn;
    osg::Object*    memloc;
    osg::Group      *group;
    osg::Geode      *geode;
    osg::Drawable   *drawable;

    memloc = node;

    if (graphnodemap.find(memloc) == graphnodemap.end())
    {
        gn = new GraphNode(node);

        group = dynamic_cast<osg::Group*>(node);
        if (group)
        {
                        // visit any children if Group (or descendant)
            for (unsigned int c=0; c < group->getNumChildren(); c++)
            {
                // store that the current node points to its child
                                gn->add_target(group->getChild(c));
                                traverse(graphnodemap, group->getChild(c));
            }
        }
        else
        {
            geode = dynamic_cast<osg::Geode*>(node);
            if (geode)
            {
                for (unsigned int di=0; di < geode->getNumDrawables(); di++)
                {
                                        drawable = geode->getDrawable(di);

                                        GraphNode *gn2 = new GraphNode(drawable);
                    osg::Object* memloc2 = drawable;

                    if (graphnodemap.find(memloc2) == graphnodemap.end())
                                                graphnodemap[memloc2] = gn2;

                                        gn->add_target(memloc2);
                }
            }
        }

        graphnodemap[memloc] = gn;
    }
}

int
main(int argc, char *argv[])
{
    osg::ref_ptr<osg::Node> root;
    GraphNodeMap            graph_nodes;
    GraphNodeMap::iterator  it;
    FILE                    *f;
    osg::Object*            memloc;
    GraphNode               *graphnode, *targetnode;
    char                    s[1024];
    std::string             shape, color, fillcolor, style, label;

    root = osgDB::readNodeFile(argv[1]);

    traverse(graph_nodes, root.get());

    printf("%d graph nodes\n", graph_nodes.size());

    f = fopen("graph.dot", "wt");

    fprintf(f, "digraph osg_scenegraph {\n  rankdir = LR;\n");

    // first, output a list of graph nodes, with their labels
    for (it = graph_nodes.begin(); it != graph_nodes.end(); ++it)
    {
        memloc = it->first;
        graphnode = it->second;

        //shape = "box";

        color = "black";
        fillcolor = "white";

        if (graphnode->type == "node")
            style = "setlinewidth(2)";
        else if (graphnode->type == "drawable")
            style = "solid";

        if (graphnode->has_stateset)
        {
            style += ",filled";
            fillcolor = "gray90";
        }

        shape = "record";

        sprintf(s, "<top> %s", graphnode->osg_class.c_str());
        label = std::string(s);

        if (graphnode->name != "")
        {
            sprintf(s, "| %s", graphnode->name.c_str());
            label += std::string(s);
        }

        if (graphnode->extra_label != "")
        {
            sprintf(s, "| %s", graphnode->extra_label.c_str());
            label += std::string(s);
            shape = "record";
        }

        sprintf(s, "%s [shape=%s, label=\"%s\", style=\"%s\", color=\"%s\", fillcolor=\"%s\"];",
            graphnode->dot_id.c_str(), shape.c_str(), label.c_str(), style.c_str(), color.c_str(), fillcolor.c_str());
        fprintf(f, "%s\n", s);
    }

    // next, output connections
    std::vector<osg::Object*>::iterator memlocit;
    osg::Object* target_memloc;

    for (it = graph_nodes.begin(); it != graph_nodes.end(); ++it)
    {
        graphnode = it->second;

        for (memlocit = graphnode->target_mem_locations.begin();
             memlocit != graphnode->target_mem_locations.end();
             ++memlocit)
        {
            target_memloc = *memlocit;

            targetnode = graph_nodes[target_memloc];

            if (targetnode->type == "drawable")
                sprintf(s, "%s:top -> %s:top [style=dashed];", graphnode->dot_id.c_str(), targetnode->dot_id.c_str());
            else
                sprintf(s, "%s:top -> %s:top [style=\"setlinewidth(2)\"];", graphnode->dot_id.c_str(), targetnode->dot_id.c_str());
            fprintf(f, "%s\n", s);
        }
     }

    // done!
    fprintf(f, "}\n");

    fclose(f);
}

