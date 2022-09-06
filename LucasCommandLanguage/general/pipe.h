#ifndef PIPE_DOT_H
#define PIPE_DOT_H

#ifndef abstract
#define abstract = 0
#endif

namespace pipe {

    template <typename P, typename R>
    struct Functor {
        virtual R operator () (P p) const abstract;
        virtual ~Functor() { }
    };

    template <typename P, typename R>
    auto operator % (const P& parameter, const Functor<P, R>& functor) {
        return functor(parameter);
    }

}

#endif