#ifndef CIGMAR_INPLACE_UNARY_FUNCTION
#define CIGMAR_INPLACE_UNARY_FUNCTION

template<typename T>
class InplaceUnaryFunction {
public:
	virtual void operator()(T&) = 0;
};

#endif // CIGMAR_INPLACE_UNARY_FUNCTION
