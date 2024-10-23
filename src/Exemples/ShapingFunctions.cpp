#include <Bitmap.hpp>
#include <cmath>
#include <Maths.hpp>

int main(int argc, char **argv) {
    Bitmap bmpMAL(512, 512, false);

    for (int y = 0; y < bmpMAL.getHeight(); y++) {
        for (int x = 0; x < bmpMAL.getWidth(); x++) {
          
            double c = Step(0.5, static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth())) * 255.;
            //double c = Smoothstep_One(0., 1., static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth())) * 255.;
            //double c = Smoothstep_Two(0., 1., static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth())) * 255.;
            //double c = Clamp(Sin(1., 150., static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth())) * 255., 0., 255.);
            //double c = Max(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.5) * 255.;
            //double c = Floor(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth())) * 255.;    
            //double c = Ceil(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth())) * 255.;    
            //double c = ExpStep(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.5) * 255.;    
            //double c = RationalBump(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.2) * 255.;    
            //double c = CubicPulse(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.5, 0.2) * 255.;    
            //double c = Pcurve(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 3., 1.) * 255.;    
            //double c = Parabola(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 1.) * 255.;    
            //double c = Gain(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.5) * 255.;    
            //double c = AlmostUnitIdentity(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth())) * 255.;    
            //double c = Trunc_fallof(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.5) * 255.;    
            //double c = Sinc(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.1) * 255.;    
            //double c = ExpSustainedImpulse(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.1, 0.5) * 255.;    
            //double c = PolyImpulse(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.3, 5.) * 255.;    
            //double c = QuaImpulse(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.8) * 255.;    
            //double c = ExpImpulse(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.5) * 255.;    
            //double c = IntegralSmoothstep(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.5) * 255.;    
            //double c = AlmostIdentityV1(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.2, 0.6) * 255.;    
            //double c = AlmostIdentityV2(static_cast<double>(x) / static_cast<double>(bmpMAL.getWidth()), 0.5) * 255.;    


            std::cout << c <<std::endl;
            bmpMAL.normalize(&c);
            bmpMAL.set_pixel(c, c, c, 1., x, y);
        }
    }

    bmpMAL.write("ecriture.bmp");

    return 0;
}