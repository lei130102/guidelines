#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include <string>
#include <iostream>

class Context;

class State
{
protected:
	Context* context_;

public:
	virtual ~State()
	{}

	void set_context(Context* context)
	{
		this->context_ = context;
	}

	//状态模式的缺点是限定死了所有状态都必须有统一的接口，如下：
	virtual void Handle1() = 0;
	virtual void Handle2() = 0;
};

class Context
{
private:
	State* state_;

public:
	Context(State* state) : state_(nullptr)
	{
		this->TransitionTo(state);
	}

	~Context()
	{
		delete state_;
	}

	void TransitionTo(State* state)
	{
		std::cout << "Context:Transition to " << typeid(*state).name() << ".\n";
		if (this->state_ != nullptr)
		{
			delete this->state_;
		}
		this->state_ = state;
		this->state_->set_context(this);
	}

	void Request1()
	{
		this->state_->Handle1();
	}

	void Request2()
	{
		this->state_->Handle2();
	}
};

class ConcreteStateA : public State
{
public:
	void Handle1() override;

	void Handle2() override
	{
		std::cout << "ConcreteStateA handles request2.\n";
	}
};

class ConcreteStateB : public State
{
public:
	void Handle1() override
	{
		std::cout << "ConcreteStateB handles request1.\n";
	}

	void Handle2() override
	{
		std::cout << "ConcreteStateB handles request2.\n";
		std::cout << "ConcreteStateB wants to change the state of the context.\n";
		//状态模式与策略模式相似，但有一个关键性的不同——在状态模式中，特定状态知道其他所有状态的存在，
		//且能触发从一个状态到另一个状态的转换；策略则几乎完全不知道其他策略的存在
		this->context_->TransitionTo(new ConcreteStateA);
	}
};

void ConcreteStateA::Handle1()
{
	std::cout << "ConcreteStateA handles request1.\n";
	std::cout << "ConcreteStateA wants to change the state of the context.\n";
}

void ClientCode()
{
	Context* context = new Context(new ConcreteStateA);
	context->Request1();
	context->Request2();
	delete context;
}

BOOST_AUTO_TEST_SUITE(s)

BOOST_AUTO_TEST_CASE(c)
{
	ClientCode();
}

BOOST_AUTO_TEST_SUITE_END()