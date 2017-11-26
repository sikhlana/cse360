<?php

namespace App;

use Illuminate\Database\Eloquent\Model;

class Variable extends Model
{
    public $timestamps = false;

    protected $primaryKey = 'name';
    public $incrementing = false;

    protected $fillable = [
        'name', 'value',
    ];
}
