# pimpl
Implementation of C++'s pointer to implementation idiom

I like separation of interface from implementation, but dislike using heap allocation and a unique pointer to achieve it. This repo (will at some point) hold an implementation that doesn't require heap allocation. 

Test cases use catch, see https://github.com/philsquared/Catch

The pimpl idiom is a strategy for separating interface and implementation of a class. This is popular as a means of decoupling compilation dependencies. It is also a mechanism for defining a class which is simultaneously easy to test and defines a minimal extrenal interface.

The assumption is that the application code will contain two classes.

One class defines an interface as a set of methods. The intention is for there to be no directly accessible state. Inheriting from pimpl::base defines forwarding constructors. I am considering exposing the standard set of 38(ish) operators as well.

The second class defines code of whatever arbitrary complexity you wish, including visible state, subclasses etc. This pimpl utility can help glue the two together.

Current functionality:
No heap allocation
No global state
Constructor forwarding
Destructor forwarding
Copy / copy assign / move / move assign forwarding
Some operator forwarding - not entirely sure this is worthwhile

Current limitations:

The base class specialisation needs to be explicitly instantiated in the source file, e.g. template class base<example_impl, sizeof(example_impl), alignof(example_impl)>;

Instantiations corresponding to the implementation's constructors are required, e.g.
  template targ::base(int &&, int&&);
  
The author is not entirely convinced this is a sensible strategy and may abandon this.