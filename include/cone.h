#ifndef CONE_H
#define CONE_H


class cone
{

    public:
        cone();
        int vertexCount;
        float *vertices;
        float *texCoords;
        float *normals;
        void zrob_tablice();
        void drawCone();



        virtual ~cone();

    protected:

    private:
};

namespace coneNamespace {
            extern float vertices[];
            extern float texCoords[];
            extern float normals[];
        };

#endif // CONE_H
