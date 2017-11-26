<?php

namespace App\Http\Resources;

use Illuminate\Http\Resources\Json\Resource;

class VariableResource extends Resource
{
    /**
     * Transform the resource into an array.
     *
     * @param  \Illuminate\Http\Request
     * @return array
     */
    public function toArray($request)
    {
        $arr = parent::toArray($request);

        if (is_numeric($arr['value']))
        {
            $arr['value'] = floatval($arr['value']);
        }

        return $arr;
    }
}
