#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

class Renderer {
    public:
        Renderer();
        
        // This function is called at the beginning of the frame that you are rendering. 
        void prepare(SDL_Surface* canvas, SDL_Rect viewPortRect, Basis camera);
        
        template <typename ColorPointIterator>
        void renderPoint(ColorPointIterator begin, ColorPointIterator end) const {
            for (ColorPointIterator iter = begin; iter != end; ++iter) {
                Point p = static_cast<Point&>(*iter);
                SDL_Color color = iter->color;
                
                p = cameraMatrix * p;
                if (p.z < 0) {
                    continue;
                }
                p = projectionMatrix * p;
        
                // Any point out of bounds of the view rectangle is skipped.
                if (p.x < viewPortRect.x ||
                    p.y < viewPortRect.y ||
                    p.x >= viewPortRect.x + viewPortRect.w ||
                    p.y >= viewPortRect.y + viewPortRect.h) {
                        continue;
                }
        
                // Render points that weren't skipped.
                // Offset formula:
                // width*y+x
                unsigned int offset = canvas->w * static_cast<unsigned int>(p.y) + static_cast<unsigned int>(p.x);
                pixels[offset] = SDL_MapRGBA(canvas->format, currentPoint.color.r, currentPoint.color.g, currentPoint.color.b, currentPoint.color.a);
            }
        }
    private:
        const double focalDistance = 60;
};

#endif // RENDER_H_INCLUDED