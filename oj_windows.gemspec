# frozen_string_literal: true

require 'date'
require File.join(File.dirname(__FILE__), 'lib/oj_windows/version')

Gem::Specification.new do |s|
  s.name = 'oj_windows'
  s.version = Oj::VERSION
  s.authors = ['Peter Ohler', 'tigel']
  s.email = 'tigel-agm@tigel.com'
  s.homepage = 'https://github.com/tigel-agm/oj_windows'
  s.metadata = {
    'bug_tracker_uri' => 'https://github.com/tigel-agm/oj_windows/issues',
    'changelog_uri' => 'https://github.com/tigel-agm/oj_windows/blob/main/CHANGELOG.md',
    'documentation_uri' => 'https://github.com/tigel-agm/oj_windows/blob/main/README.md',
    'source_code_uri' => 'https://github.com/tigel-agm/oj_windows',
    'rubygems_mfa_required' => 'true'
  }
  s.summary = 'A build of the Oj JSON parser for Ruby compiled with the MSVC (mswin) toolchain'
  s.description = 'oj_windows is a fork of Oj (https://github.com/ohler55/oj) by Peter Ohler, ' \
                  'adapted to build with the MSVC (mswin) Ruby toolchain: pthread mutexes replaced with ' \
                  'Windows primitives, POSIX headers guarded, and SIMD string scanning enabled under MSVC. ' \
                  'It provides the full Oj API (module Oj) and JSON-gem compatibility. Because it defines the ' \
                  'same Oj module, it is a replacement for - and cannot be installed alongside - the upstream ' \
                  'oj gem. Requires Ruby 3.4+ built with the MSVC toolchain (RubyInstaller/MinGW users should ' \
                  'use the upstream oj gem instead).'
  s.licenses = ['MIT']
  s.required_ruby_version = '>= 3.4'

  # Ship only the active oj_windows tree (lib/oj/ is a dead, byte-identical
  # duplicate carried over from the rename and is intentionally excluded).
  s.files = Dir['lib/oj_windows.rb', 'lib/oj_windows/**/*.rb', 'ext/**/*.{c,h,rb}'] +
            ['LICENSE', 'README.md', 'CHANGELOG.md'] + Dir['pages/*.md']
  s.extensions = ['ext/oj_windows/extconf.rb']

  s.extra_rdoc_files = ['README.md', 'LICENSE', 'CHANGELOG.md'] + Dir['pages/*.md']
  s.rdoc_options = ['--title', 'oj_windows', '--main', 'README.md']

  s.add_runtime_dependency 'bigdecimal', '>= 3.0'
  s.add_runtime_dependency 'ostruct', '>= 0.2'
  s.add_development_dependency 'minitest', '~> 5'
  s.add_development_dependency 'rake-compiler', '>= 0.9', '< 2.0'
  s.add_development_dependency 'test-unit', '~> 3.0'
end
