# Laravel Search Lang

The tool for scan Laravel project for obtain all texts which will need translations.

The tool is scanning for `__()` and `@lang()` phrases in all PHP files found in given path.

## How to build

Download the project and in CLI type:

```bash
make
```

As the result the *laravel-search-lang* file will be created.

## Usage

Call *laravel-search-lang* with the path to Laravel project:

```bash
laravel-search-lang <path to laravel> [-o <output file>]
```

For example:

```bash
laravel-search-lang /var/www/html/my-laravel --output /var/www/html/my-laravel/resources/lang/pl.json
```

As result the JSON file will be created with all found texts to translate.

If the output JSON file already exist then translation texts will be merged, so you shouldn't lose your current translations.

