/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_op_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcassier <tcassier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/03/01 16:06:31 by tcassier          #+#    #+#             */
/*   Updated: 2018/04/02 14:56:43 by tcassier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static char	*asm_op_params_jump(t_asm *data, char *tmp, int idx_param)
{
	if (idx_param < g_op_tab[data->idx].nb_params - 1)
	{
		while (!ft_isspace(*tmp) && *tmp != ',' && *tmp !=
		COMMENT_CHAR && *tmp)
			tmp++;
		tmp = ft_spacejump(tmp);
		if (*tmp != ',')
			asm_error_7(data);
		tmp++;
	}
	else
	{
		while (!ft_isspace(*tmp) && *tmp && *tmp != ',' && *tmp != COMMENT_CHAR)
			tmp++;
		if (*tmp == ',')
			asm_error_7(data);
		tmp = ft_spacejump(tmp);
		if (*tmp != COMMENT_CHAR && *tmp)
			asm_error_7(data);
	}
	return (tmp);
}

static void	asm_op_params(t_asm *data, char *tmp)
{
	int		ocp;
	int		check;

	data->param = 0;
	ocp = 0;
	if (g_op_tab[data->idx].has_ocp)
	{
		check = data->size;
		data->size += 1;
	}
	while (data->param < g_op_tab[data->idx].nb_params)
	{
		tmp = ft_spacejump(tmp);
		if (*tmp == '%' || *tmp == 'r' || *tmp == '-' || *tmp == ':' ||
		ft_isdigit(*tmp))
			ocp |= asm_op_type_param(data, data->param, tmp);
		else
			asm_error_7(data);
		tmp = asm_op_params_jump(data, tmp, data->param);
		data->param++;
	}
	if (g_op_tab[data->idx].has_ocp)
		asm_put_hexa(data->file + check, ocp, 1);
}

void		asm_op_line(t_asm *data, char *tmp)
{
	int		idx;

	idx = 0;
	while (g_op_tab[idx].name)
	{
		if (!ft_strncmp(g_op_tab[idx].name, tmp, ft_strlen(g_op_tab[idx].name))
		&& ft_isspace(tmp[ft_strlen(g_op_tab[idx].name)]))
			break ;
		idx++;
	}
	if (idx == 16)
		asm_error_11(data);
	else
	{
		data->file[data->size] = g_op_tab[idx].opcode;
		data->pc = data->size;
		data->size++;
		while (!ft_isspace(*tmp) && *tmp)
			tmp++;
		data->idx = idx;
		asm_op_params(data, tmp);
	}
}
