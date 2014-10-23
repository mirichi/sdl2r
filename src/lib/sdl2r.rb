require 'sdl2r.so'

module SDL
  class Rect < Array
    def initialize(x, y, w, h)
      super(4)
      self[0] = x
      self[1] = y
      self[2] = w
      self[3] = h
    end
    def x;self[0];end
    def x=(v);self[0]=v;end
    def y;self[1];end
    def y=(v);self[1]=v;end
    def w;self[2];end
    def w=(v);self[2]=v;end
    def h;self[3];end
    def h=(v);self[3]=v;end
  end

  class Point < Array
    def initialize(x, y)
      super(2)
      self[0] = x
      self[1] = y
    end
    def x;self[0];end
    def x=(v);self[0]=v;end
    def y;self[1];end
    def y=(v);self[1]=v;end
  end

  class Color < Array
    def initialize(r, g, b, a=255)
      super(4)
      self[0] = r
      self[1] = g
      self[2] = b
      self[3] = a
    end
    def r;self[0];end
    def r=(v);self[0]=v;end
    def g;self[1];end
    def g=(v);self[1]=v;end
    def b;self[2];end
    def b=(v);self[2]=v;end
    def a;self[3];end
    def a=(v);self[3]=v;end
  end
end

