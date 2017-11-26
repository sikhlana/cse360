<?php

use App\Variable;
use Illuminate\Database\Seeder;

class VariablesTableSeeder extends Seeder
{
    /**
     * Run the database seeds.
     *
     * @return void
     */
    public function run()
    {
        \DB::table('variables')->truncate();

        Variable::create([
            'name' => 'preferred_temperature',
            'value' => 35,
        ]);

        Variable::create([
            'name' => 'current_temperature',
            'value' => 0,
        ]);

        Variable::create([
            'name' => 'pump',
            'value' => 'off',
        ]);

        Variable::create([
            'name' => 'heater',
            'value' => 'off',
        ]);

        Variable::create([
            'name' => 'cooler',
            'value' => 'off',
        ]);
    }
}
