<?php

namespace App\Http\Controllers;

use App\Http\Resources\VariableResource;
use App\Variable;
use Illuminate\Http\Request;

class VariableController extends Controller
{
    public function index()
    {
        $variables = Variable::all();
        $output = [];

        /** @var Variable $variable */
        foreach ($variables as $variable)
        {
            $output[$variable->name] = VariableResource::make($variable);
        }

        return response()->json($output);
    }

    public function update(Variable $variable, Request $request)
    {
        $this->validate($request, [
            'value' => 'required',
        ]);

        $variable->value = $request->input('value');
        $variable->save();

        return VariableResource::make($variable);
    }
}
