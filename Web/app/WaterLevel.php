<?php

namespace App;

use Illuminate\Database\Eloquent\Model;

class WaterLevel extends Model
{
    public $timestamps = false;

    protected $fillable = [
        'level', 'timestamp',
    ];

    protected $casts = [
        'timestamp' => 'datetime',
    ];
}
