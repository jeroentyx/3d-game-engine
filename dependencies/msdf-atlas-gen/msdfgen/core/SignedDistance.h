
#pragma once

#ifdef INFINITE
    #undef INFINITE
#endif

namespace msdfgen {

/// Represents a signed distance and alignment, which together can be compared to uniquely determine the closest edge segment.
class SignedDistance {

public:
    static const SignedDistance INFINITE;

    double distance;
    double dot;

    SignedDistance();
    SignedDistance(double dist, double d);

    friend bool operator<(SignedDistance a, SignedDistance b);
    friend bool operator>(SignedDistance a, SignedDistance b);
    friend bool operator<=(SignedDistance a, SignedDistance b);
    friend bool operator>=(SignedDistance a, SignedDistance b);

};

}
