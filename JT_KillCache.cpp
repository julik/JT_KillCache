
static const char* const HELP = "JT_KillCaches destroys the Nuke caching pipe";

#include "DDImage/PixelIop.h"
#include "DDImage/Row.h"
#include "DDImage/Knobs.h"

using namespace DD::Image;

class JT_KillCache : public PixelIop
{
  float value[4];
  
  
public:
  void in_channels(int input, ChannelSet& mask) const;
  JT_KillCache(Node* node) : PixelIop(node)
  {
    value[0] = value[1] = value[2] = value[3] = 0;
  }
  bool pass_transform() const { return true; }
  void pixel_engine(const Row &in, int y, int x, int r, ChannelMask, Row & out);
  
  virtual void knobs(Knob_Callback);
  
  static const Iop::Description d;
  
  const char* Class() const { return d.name; }
  const char* node_help() const { return HELP; }
  void _validate(bool);
  
    void append(Hash& hash) {
        hash.append(rand());
     }
};

void JT_KillCache::_validate(bool for_real)
{
  copy_info();
  for (unsigned i = 0; i < 4; i++) {
    if (value[i]) {
      set_out_channels(Mask_All);
      info_.black_outside(false);
      return;
    }
  }
  set_out_channels(Mask_None);
}

void JT_KillCache::in_channels(int input, ChannelSet& mask) const
{
  // mask is unchanged
}

void JT_KillCache::pixel_engine(const Row& in, int y, int x, int r,
                       ChannelMask channels, Row& out)
{
  foreach (z, channels) {
    const float* inptr = in[z] + x;
    const float* END = inptr + (r - x);
    float* outptr = out.writable(z) + x;
    while (inptr < END)
      *outptr++ = *inptr++;
  }
}

void JT_KillCache::knobs(Knob_Callback f)
{
  //AColor_knob(f, value, IRange(0, 4), "value");
}

// Since we do not need channel selectors or masks, we can use our raw Iop
// directly instead of putting it into a NukeWrapper. Besides, the mask input on
// the NukeWrapper cannot be disabled even though the Foundry doco says it can
// (Foundry bug #12598)
static Iop* JT_KillCacheCreate( Node* node ) {
	return new JT_KillCache(node);
}

const Iop::Description JT_KillCache::d("JT_KillCache", "Color/Math/JT_KillCache", JT_KillCacheCreate);


