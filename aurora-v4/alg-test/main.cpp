#include "aurora-v4/aurora.h"
#include <iostream>
#include "affix-base/stopwatch.h"

using namespace aurora;

void gradient_descent_test(

)
{
	std::vector<parameter*> l_inputs;
	std::vector<parameter*> l_parameters;
	std::vector<affix_base::data::ptr<element>> l_render_output;

	affix_base::data::ptr<parameter> l_x = new parameter(l_render_output, l_inputs);
	affix_base::data::ptr<power> l_power = new power(l_render_output, &l_x->m_y, &(new parameter(l_render_output, l_parameters))->m_y);
	affix_base::data::ptr<weight> l_multiply = new weight(l_render_output, l_parameters, &l_power->m_y);
	affix_base::data::ptr<bias> l_add = new bias(l_render_output, l_parameters, l_multiply->m_y);
	affix_base::data::ptr<divide> l_divide = new divide(l_render_output, l_add->m_y, &(new parameter(l_render_output, l_parameters))->m_y);

	for (int i = 0; i < l_parameters.size(); i++)
		l_parameters[i]->m_y.m_state = 0.01 + (double)i * 0.01;

	for (int epoch = 0; epoch < 1000; epoch++)
	{
		l_x->m_y.m_state = 10;

		for (auto& l_element : l_render_output)
			l_element->fwd();

		l_divide->m_y.m_gradient = l_divide->m_y.m_state - -5;

		std::cout << l_divide->m_y.m_gradient << std::endl;

		for (int i = l_render_output.size() - 1; i >= 0; i--)
			l_render_output[i]->bwd();

		for (parameter* l_param : l_parameters)
			l_param->m_y.m_state -= 0.002 * l_param->m_y.m_gradient;

	}
}

void weight_set_test(

)
{
	std::vector<affix_base::data::ptr<element>> l_elements;
	std::vector<parameter*> l_parameters;
	std::vector<parameter*> l_inputs;

	state_gradient_pair l_x_0 = { 10, 0 };
	state_gradient_pair l_x_1 = { 11, 0 };
	state_gradient_pair l_x_2 = { 12, 0 };
	state_gradient_pair l_x_3 = { 13, 0 };

	std::vector<state_gradient_pair*> l_x = { &l_x_0, &l_x_1, &l_x_2, &l_x_3 };

	weight_junction l_weight_junction(l_elements, l_parameters, l_x, 10);

	for (int i = 0; i < l_parameters.size(); i++)
		l_parameters[i]->m_y.m_state = 2 + ((double)i * 0.01);

	for (int i = 0; i < l_elements.size(); i++)
		l_elements[i]->fwd();

	for (int i = 0; i < l_weight_junction.m_y.size(); i++)
	{
		l_weight_junction.m_y[i]->m_gradient = 10;
	}

	for (int i = l_elements.size() - 1; i >= 0; i--)
		l_elements[i]->bwd();

}

void additive_aggregate_test(

)
{
	std::cout << "ADDITIVE AGGREGATE TEST" << std::endl;
	std::vector<affix_base::data::ptr<element>> l_elements;
	std::vector<parameter*> l_parameters;

	std::vector<state_gradient_pair> l_x = { {1}, {2}, {3}, {4} };

	additive_aggregate l_aggregate(l_elements, { &l_x[0], &l_x[1], &l_x[2], &l_x[3] });

	for (int epoch = 0; epoch < 10000; epoch++)
	{
		for (int i = 0; i < l_elements.size(); i++)
			l_elements[i]->fwd();

		l_aggregate.m_y->m_gradient = l_aggregate.m_y->m_state - 20.0;

		if (epoch % 100 == 0)
			std::cout << l_aggregate.m_y->m_gradient << std::endl;

		for (int i = l_elements.size() - 1; i >= 0; i--)
			l_elements[i]->bwd();

		for (int i = 0; i < l_x.size(); i++)
		{
			l_x[i].m_state -= 0.002 * l_x[i].m_gradient;
			l_x[i].m_gradient = 0;
		}
	}

}

void multiplicative_aggregate_test(

)
{
	std::cout << "MULTIPLICATIVE AGGREGATE TEST" << std::endl;
	std::vector<affix_base::data::ptr<element>> l_elements;
	std::vector<parameter*> l_parameters;

	std::vector<state_gradient_pair> l_x = { {1}, {2}, {3}, {4} };

	multiplicative_aggregate l_aggregate(l_elements, { &l_x[0], &l_x[1], &l_x[2], &l_x[3] });

	for (int epoch = 0; epoch < 1000; epoch++)
	{
		for (int i = 0; i < l_elements.size(); i++)
			l_elements[i]->fwd();

		l_aggregate.m_y->m_gradient = l_aggregate.m_y->m_state - 20.0;

		if (epoch % 100 == 0)
			std::cout << l_aggregate.m_y->m_gradient << std::endl;

		for (int i = l_elements.size() - 1; i >= 0; i--)
			l_elements[i]->bwd();

		for (int i = 0; i < l_x.size(); i++)
		{
			l_x[i].m_state -= 0.002 * l_x[i].m_gradient;
			l_x[i].m_gradient = 0;
		}

	}

}

void tanh_activate_test(

)
{
	std::vector<affix_base::data::ptr<element>> l_elements;

	state_gradient_pair l_x = { 3 };

	affix_base::data::ptr<tanh_activate> l_tanh_activate(new tanh_activate(l_elements, &l_x));

	for (int epoch = 0; epoch < 100000; epoch++)
	{
		for (int i = 0; i < l_elements.size(); i++)
			l_elements[i]->fwd();

		l_tanh_activate->m_y.m_gradient = l_tanh_activate->m_y.m_state - (-0.5);

		if (epoch % 100 == 0)
			std::cout << l_tanh_activate->m_y.m_gradient << std::endl;

		for (int i = l_elements.size() - 1; i >= 0; i--)
			l_elements[i]->bwd();

		l_x.m_state -= 0.02 * l_x.m_gradient;
		l_x.m_gradient = 0;
	}

}

void leaky_relu_activate_test(

)
{
	std::vector<affix_base::data::ptr<element>> l_elements;

	state_gradient_pair l_x = { 3 };

	affix_base::data::ptr<leaky_relu_activate> l_leaky_relu_activate(new leaky_relu_activate(l_elements, &l_x, 0.3));

	for (int epoch = 0; epoch < 100000; epoch++)
	{
		for (int i = 0; i < l_elements.size(); i++)
			l_elements[i]->fwd();

		l_leaky_relu_activate->m_y.m_gradient = l_leaky_relu_activate->m_y.m_state + 20;

		if (epoch % 100 == 0)
			std::cout << l_leaky_relu_activate->m_y.m_gradient << std::endl;

		for (int i = l_elements.size() - 1; i >= 0; i--)
			l_elements[i]->bwd();

		l_x.m_state -= 0.02 * l_x.m_gradient;
		l_x.m_gradient = 0;
	}

}

void normalize_test(

)
{
	std::vector<affix_base::data::ptr<element>> l_elements;

	std::vector<state_gradient_pair> l_x = { {1}, {2}, {3} };

	normalize l_normalize(l_elements, { &l_x[0], &l_x[1], &l_x[2] });

	for (int epoch = 0; epoch < 10000; epoch++)
	{
		for (int i = 0; i < l_elements.size(); i++)
			l_elements[i]->fwd();

		double l_cost = 0;

		for (int i = 0; i < l_normalize.m_y.size(); i++)
		{
			l_normalize.m_y[i]->m_gradient = l_normalize.m_y[i]->m_state - 0.333333333333;
			l_cost += abs(l_normalize.m_y[i]->m_gradient);
		}

		if (epoch % 100 == 0)
			std::cout << l_cost << std::endl;

		for (int i = l_elements.size() - 1; i >= 0; i--)
			l_elements[i]->bwd();

		for (int i = 0; i < l_x.size(); i++)
			l_x[i].m_state -= 0.02 * l_x[i].m_gradient;

	}

}

void parameterized_interpolate_test(

)
{
	std::vector<affix_base::data::ptr<element>> l_elements;
	std::vector<parameter*> l_parameters;

	std::vector<state_gradient_pair> l_x = { {1}, {20}, {30} };

	parameterized_interpolate l_parameterized_interpolate(l_elements, l_parameters, { &l_x[0], &l_x[1], &l_x[2] });

	for (int i = 0; i < l_parameters.size(); i++)
		l_parameters[i]->m_y.m_state = (double)i * 0.01;

	for (int epoch = 0; epoch < 1000000; epoch++)
	{
		for (int i = 0; i < l_elements.size(); i++)
			l_elements[i]->fwd();

		l_parameterized_interpolate.m_y->m_gradient = l_parameterized_interpolate.m_y->m_state - 20;

		if (epoch % 1 == 0)
			std::cout << l_parameterized_interpolate.m_y->m_gradient << std::endl;

		for (int i = l_elements.size() - 1; i >= 0; i--)
			l_elements[i]->bwd();

		for (int i = 0; i < l_parameters.size(); i++)
			l_parameters[i]->m_y.m_state -= 0.02 * l_parameters[i]->m_y.m_gradient;

	}

}

void tnn_test(

)
{
	std::vector<affix_base::data::ptr<element>> l_elements;
	std::vector<parameter*> l_parameters;

	std::vector<state_gradient_pair> l_x(2);

	tnn l_tnn(l_elements, l_parameters, pointers_to_each_element(l_x),
		{
			tnn::layer_info(5, neuron_tanh(l_elements, l_parameters)),
			tnn::layer_info(1, neuron_sigmoid(l_elements, l_parameters))
		}
	);

	std::uniform_real_distribution<double> l_urd(-1, 1);
	std::default_random_engine l_dre(25);

	for (int i = 0; i < l_parameters.size(); i++)
	{
		l_parameters[i]->m_y.m_state = l_urd(l_dre);
	}

	auto l_cycle = [&](const std::vector<double>& a_x, const std::vector<double>& a_y)
	{
		for (int i = 0; i < l_x.size(); i++)
			l_x[i].m_state = a_x[i];

		for (int i = 0; i < l_elements.size(); i++)
			l_elements[i]->fwd();

		double l_cost = 0;

		for (int i = 0; i < l_tnn.m_y.size(); i++)
		{
			l_tnn.m_y[i]->m_gradient = l_tnn.m_y[i]->m_state - a_y[i];
			l_cost += abs(l_tnn.m_y[i]->m_gradient);
		}

		for (int i = l_elements.size() - 1; i >= 0; i--)
			l_elements[i]->bwd();

		return l_cost;

	};

	const int CHECKPOINT = 100000;

	std::vector<std::vector<double>> l_ts_x = {
		{0, 0},
		{0, 1},
		{1, 0},
		{1, 1}
	};

	std::vector<std::vector<double>> l_ts_y = {
		{0},
		{1},
		{1},
		{0}
	};

	affix_base::timing::stopwatch l_stopwatch;
	l_stopwatch.start();

	for (int epoch = 0; epoch < 1000000; epoch++)
	{
		double l_cost = 0;

		l_cost += l_cycle(l_ts_x[0], l_ts_y[0]);

		if (epoch % CHECKPOINT == 0)
			std::cout << l_tnn.m_y[0]->m_state << std::endl;

		l_cost += l_cycle(l_ts_x[1], l_ts_y[1]);

		if (epoch % CHECKPOINT == 0)
			std::cout << l_tnn.m_y[0]->m_state << std::endl;

		l_cost += l_cycle(l_ts_x[2], l_ts_y[2]);

		if (epoch % CHECKPOINT == 0)
			std::cout << l_tnn.m_y[0]->m_state << std::endl;

		l_cost += l_cycle(l_ts_x[3], l_ts_y[3]);

		if (epoch % CHECKPOINT == 0)
			std::cout << l_tnn.m_y[0]->m_state << std::endl;

		for (int i = 0; i < l_parameters.size(); i++)
		{
			l_parameters[i]->m_y.m_state -= 0.002 * l_parameters[i]->m_y.m_gradient;
			l_parameters[i]->m_y.m_gradient = 0;
		}

		if (epoch % CHECKPOINT == 0)
		{
			std::cout << std::endl;
			std::cout << l_cost << std::endl << std::endl;
		}

	}

	std::cout << std::endl << l_stopwatch.duration_milliseconds() << std::endl;

}

double sign_d(const double& a_double)
{
	if (a_double >= 0)
		return 1.0;
	else
		return -1.0;
}

void parabola_test(

)
{
	std::vector<affix_base::data::ptr<element>> l_elements;
	std::vector<parameter*> l_parameters;

	std::vector<state_gradient_pair> l_x(1);

	tnn l_tnn(l_elements, l_parameters, pointers_to_each_element(l_x),
		{
			tnn::layer_info(20, neuron_leaky_relu(l_elements, l_parameters)),
			tnn::layer_info(1, neuron_leaky_relu(l_elements, l_parameters))
		});

	std::uniform_real_distribution<double> l_urd(-1, 1);
	std::default_random_engine l_dre(25);

	for (int i = 0; i < l_parameters.size(); i++)
	{
		l_parameters[i]->m_y.m_state = l_urd(l_dre);
	}

	auto l_cycle = [&](const std::vector<double>& a_x, const std::vector<double>& a_y)
	{
		for (int i = 0; i < l_x.size(); i++)
			l_x[i].m_state = a_x[i];

		for (int i = 0; i < l_elements.size(); i++)
			l_elements[i]->fwd();

		double l_cost = 0;

		for (int i = 0; i < l_tnn.m_y.size(); i++)
		{
			l_tnn.m_y[i]->m_gradient = l_tnn.m_y[i]->m_state - a_y[i];
			l_cost += abs(l_tnn.m_y[i]->m_gradient);
		}

		for (int i = l_elements.size() - 1; i >= 0; i--)
			l_elements[i]->bwd();

		return l_cost;

	};

	std::uniform_real_distribution<double> l_ts_urd(-10, 10);

	double l_cost_momentum = 0;

	const size_t CHECKPOINT_INTERVAL = 10000;

	for (int epoch = 0; true; epoch++)
	{
		double l_cost = 0;

		for (int i = 0; i < 10; i++)
		{
			double l_ts_x = l_ts_urd(l_dre);
			double l_ts_y = l_ts_x * l_ts_x;
			l_cost += l_cycle({ l_ts_x }, { l_ts_y });

			if (epoch % CHECKPOINT_INTERVAL == 0)
				std::cout << "INPUT: " << l_ts_x << ", PREDICTION: " << l_tnn.m_y[0]->m_state << ", DESIRED: " << l_ts_y << std::endl;

		}

		l_cost_momentum = 0.99 * l_cost_momentum + 0.01 * l_cost;

		for (int i = 0; i < l_parameters.size(); i++)
		{
			l_parameters[i]->m_y.m_state -= 0.002 * tanh(l_parameters[i]->m_y.m_gradient);
			l_parameters[i]->m_y.m_gradient = 0;
		}

		if (epoch % CHECKPOINT_INTERVAL == 0)
			std::cout << "    LOSS FOR ABOVE EPOCH: " << l_cost_momentum << std::endl;

	}

}

int main(

)
{
	srand(time(0));

	tnn_test();

	return 0;
}
