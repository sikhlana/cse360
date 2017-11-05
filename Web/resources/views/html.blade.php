<!doctype html>
<html id="public" lang="{{ app()->getLocale() }}">
<head>
    <meta charset="utf-8">

    <meta http-equiv="X-UA-Compatible" content="IE=Edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <meta name="site" content="{{ $title }}">
    <title>{{ $title }}</title>

    <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Dosis:300|Montserrat:600|Lato:300,400,600,700|Raleway:100">
    <link rel="stylesheet" href="{{ mix('css/app.css') }}">
</head>
<body>
    <div id="app">
        <container>
            <div class="assets-loading">
                Loading Assets...
            </div>
        </container>
    </div>

    <script src="{{ mix('js/app.js') }}"></script>
</body>
</html>